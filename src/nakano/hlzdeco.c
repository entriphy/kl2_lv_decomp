#include "common.h"

static void decode_init(HLZWork *wk);
static u16 decode_c(HLZWork *wk);
static u16 decode_p(HLZWork *wk);
static void read_p_len(HLZWork *wk, s16 nn, s16 nbit, s16 is);
static void read_c_len(HLZWork *wk);
static u16 getbit(HLZWork *wk, s16 n);
static void bitbuf_full(HLZWork *wk, s16 n);
static void decode_table(HLZWork *wk, u16 nchar, u8 *bitlen, u16 tablebits, u16 *table);

u8 *HLZDecode(u8 *src, u8 *dst, HLZWork *wk_adrs) {
    u8 *src2;
    u16 c;
    s32 i;
    
    wk_adrs->in_data_adrs = src;
    wk_adrs->block_num = *src;
    wk_adrs->in_data_adrs++;
    decode_init(wk_adrs);
    
    while (1) {
        c = decode_c(wk_adrs);
        if (wk_adrs->end_flag != 0) {
            return wk_adrs->end_flag == 1 ? dst : 0;
        }
        
        if (c < 256) {
            *dst++ = c;
        } else {
            src2 = dst - (decode_p(wk_adrs) + 1);
            for (i = 253; i < c; i++) {
                *dst++ = *src2++;
            }
        }
    }
}

static void decode_init(HLZWork *wk) {
    wk->end_flag = 0;
    wk->bit8 = 0;
    wk->bit_buf = 0;
    wk->bit_buf2 = 0;
    bitbuf_full(wk, 16);
    wk->blocksize = 0;
}

static u16 decode_c(HLZWork *wk) {
    u16 j;
    u32 mask;
    
    if (wk->blocksize == 0) {
        wk->block_num--;
        if (wk->block_num < 0) {
            wk->end_flag = 1;
            return 0;
        }
        wk->blocksize = getbit(wk, 16);
        read_p_len(wk, 19, 5, 3);
        read_c_len(wk);
        read_p_len(wk, 16, 4, -1);
        if (wk->end_flag != 0) {
            return 0;
        }
    }
    wk->blocksize--;
    j = wk->c_table[wk->bit_buf2 >> 4];
    if (j >= 0x1FE) {
        mask = 0x08;
        while (j >= 0x1FE) {
            if (wk->bit_buf2 & mask)
                j = wk->right[j];
            else
                j = wk->left[j];
            mask >>= 1;
        }
    }
    
    bitbuf_full(wk, wk->c_len[j]);
    return j;
}

static u16 decode_p(HLZWork *wk) {
    u16 j;
    u32 mask;
    
    j = wk->p_table[wk->bit_buf2 >> 8];
    if (j >= 0x10) {
        mask = 0x80;
        while (j >= 0x10) {
            if (wk->bit_buf2 & mask)
                j = wk->right[j];
            else
                j = wk->left[j];
            mask >>= 1;
        }
    }

    bitbuf_full(wk, wk->p_len[j]);
    if (j != 0) {
        j = getbit(wk, j - 1) + (1 << (j - 1));
    }
    
    return j;
}

static void read_p_len(HLZWork *wk, s16 nn, s16 nbit, s16 is) {
    s16 i;
    s16 n;
    u32 mask;
    s32 c;
    
    if (wk->end_flag == 0) {
        n = getbit(wk, nbit);
        
        if (n == 0) {
            c = getbit(wk, nbit);
            for (i = 0; i < nn; i++) {
                wk->p_len[i] = 0;
            }
            for (i = 0; i < 256; i++) {
                wk->p_table[i] = c;
            }
        } else {
            i = 0;
            while (i < n) {
                c = wk->bit_buf2 >> 0xD;
                if (c == 7) {
                    mask = 0x1000;
                    while ((mask & wk->bit_buf2) != 0) {
                        c++;
                        mask >>= 1;
                    }
                }
                if (c < 7) {
                    bitbuf_full(wk, 3);
                } else {
                    bitbuf_full(wk, c - 3);
                }
                wk->p_len[i] = c;
                i++;
                if (i == is) {
                    c = getbit(wk, 2) - 1;
                    while (c >= 0) {
                        wk->p_len[i] = 0;
                        c--;
                        i++;
                    }
                }
            }
            
            for (; i < nn; i++) {
                wk->p_len[i] = 0;
            }
            decode_table(wk, nn, wk->p_len, 8, wk->p_table);
        }
    }
}

static void read_c_len(HLZWork *wk) {
    s16 i;
    s16 n;
    u32 mask;
    s32 c;
    
    if (wk->end_flag == 0) {
        n = getbit(wk, 9);
        
        if (n == 0) {
            c = getbit(wk, 9);
            
            for (i = 0; i < 0x1FE; i++) {
                wk->c_len[i] = 0;
            }
            for (i = 0; i < 0x1000; i++) {
                wk->c_table[i] = c;
            }
        } else {
            i = 0;
            
            while (i < n) {
                c = wk->p_table[wk->bit_buf2 >> 8];

                if (c > 18) {
                    mask = 0x80;
                    while (c > 18) {
                        if (wk->bit_buf2 & mask)
                            c = wk->right[c];
                        else
                            c = wk->left[c];
                        mask >>= 1;
                    }
                }
                bitbuf_full(wk, wk->p_len[c]);
                
                if (c >= 3) {
                    wk->c_len[i] = c - 2;
                    i++;
                } else {
                    if (c == 0) {
                        c = 1;
                    } else if (c == 1) {
                        c = getbit(wk, 4) + 3;
                    } else {
                        c = getbit(wk, 9) + 20;
                    }
                    c--;
                    
                    while (c > -1) {
                        wk->c_len[i] = 0;
                        c--;
                        i++;
                    }
                }
            }
            
            for (; i < 0x1FE; i++) {
                wk->c_len[i] = 0;
            }
            decode_table(wk, 0x1FE, wk->c_len, 12, wk->c_table);
        }
    }
}

static u16 getbit(HLZWork *wk, s16 n) {
    u16 uVar1 = wk->bit_buf2 >> (0x10 - n);
    bitbuf_full(wk, n);
    return uVar1;
}

static void bitbuf_full(HLZWork *wk, s16 n) {    
    wk->bit_buf2 <<= n;
    
    while (n > wk->bit8) {
        n -= wk->bit8;
        wk->bit_buf2 |= wk->bit_buf << n;
        wk->bit_buf = *wk->in_data_adrs++;
        wk->bit8 = 8;
    }
    
    wk->bit8 -= n;
    wk->bit_buf2 |= wk->bit_buf >> wk->bit8;
}

static void decode_table(HLZWork *wk, u16 nchar, u8 *bitlen, u16 tablebits, u16 *table) {
    u16 i;
    u16 ch;
    u16 jutbits;
    u16 avail;
    u16 mask;
    u32 k;
    u32 nextcode;
    u32 len;
    u32 k2;
    u16 *p;
    
    for (i = 1; i < 17; i++) {
        wk->count[i] = 0;
    }
    for (i = 0; i < nchar; i++) {
        wk->count[bitlen[i]]++;
    }
    wk->start[1] = 0;
    for (i = 1; i < 17; i++) {
        wk->start[i + 1] = wk->start[i] + (wk->count[i] << (0x10 - (i)));
    }
    if (wk->start[17] != 0) {
        wk->end_flag = 2;
    } else {
        jutbits = 0x10 - tablebits;
        for (i = 1; i <= tablebits; i++) {
            wk->start[i] >>= jutbits;
            wk->weight[i] = 1 << (tablebits - i);
        }
        for (; i < 17; i++) {
            wk->weight[i] = 1 << (0x10 - i);
        }

        i = wk->start[tablebits + 1] >> jutbits;
        if (i != 0) {
            k = 1 << tablebits;
            for (; i != k; i++) {
                table[i] = 0;
            }
        }

        avail = nchar;
        k = 0xF;
        k -= tablebits;
        mask = 1 << k;
        for (ch = 0; ch < nchar; ch++) {
            len = bitlen[ch];
            if (len != 0) {
                nextcode = wk->start[len] + wk->weight[len];
                if (len <= tablebits) {
                    for (i = wk->start[len]; i < nextcode; i++) {
                        table[i] = ch;
                    }
                } else {
                    k2 = wk->start[len];
                    p = &table[k2 >> jutbits];
                    for (i = len - tablebits; i > 0; i--) {
                        if (*p == 0) {
                            wk->right[avail] = wk->left[avail] = 0;
                            *p = avail;
                            avail++;
                        }
                        if ((k2 & mask) != 0)
                            p = &wk->right[*p];
                        else
                            p = &wk->left[*p];
                        k2 <<= 1;
                    }
                    *p = ch;
                }
                wk->start[len] = nextcode;
            }
        }
    }
}
