#include <deci2.h>
#include "okanoyo/linux/pdriver.h"

void EventHandler(s32 event, s32 param, PDriver *pd) {
    s32 n;
    PDRIVER_OPT *opt = &pd->opt;

    switch (event) {
        case DECI2_READ:
            if ((opt->rlen + param) > BUFSZ) {
                opt->rbytes = -1;
                return;
            }
            if ((n = sceDeci2ExRecv(opt->s, &opt->rptr[opt->rlen], param)) < 0)
                opt->rbytes = -1;
            opt->rlen += n;
            break;
        case DECI2_READDONE:
            opt->rbytes = opt->rlen;
            opt->rlen = 0;
            break;
        case DECI2_WRITE:
            if ((n = sceDeci2ExSend(opt->s, opt->wptr, opt->wlen)) < 0) {
                opt->rbytes = -1;
                return;
            }
            opt->wptr += n;
            opt->wlen -= n;
            break;
        case DECI2_WRITEDONE:
            if (opt->wlen != 0) opt->rbytes = -1;
            break;
        case DECI2_CHSTATUS:
            break;

        case DECI2_ERROR:
        default:
            opt->rbytes = -1;
            break;
    }
}

s32 InitProtocolDriver(PDriver *pd) {
    pd->opt.s = sceDeci2Open(PROTO_SAMPLE, pd, (void *)EventHandler);
    return pd->opt.s;
}

void SetRWBuffAddress(void *_rp, void *_wp, PDriver *pd) {
    pd->rp = (u8 *)_rp;
    pd->wp = (u8 *)_wp;
}

s32 SystemOnHost(char *command, char *result, PDriver *pd) {
    s32 ret = 0;
    u8 *data;
    u32 datalen = 0;
    sceDeci2Hdr *hdr;
    PDRIVER_OPT *opt = &pd->opt;

    opt->wptr = UNCACHED(pd->wp);
    opt->rptr = UNCACHED(pd->rp);
    opt->wlen = 0;
    opt->rlen = 0;
    opt->rbytes = 0;

    hdr = (sceDeci2Hdr *)opt->wptr;
    data = opt->wptr + sizeof(sceDeci2Hdr) + sizeof(u32);

    while ((*data++ = *command++)) datalen++;
    *data = 0;
    datalen++;
    opt->wlen = sizeof(sceDeci2Hdr) + sizeof(u32) + datalen;

    hdr->len   = opt->wlen;
    hdr->rsvd  = 0;
    hdr->proto = PROTO_SAMPLE;
    hdr->src   = NODE_EE;
    hdr->dest  = NODE_HOST;
    *(u32 *)(hdr + 1) = 0;

    if ((ret = sceDeci2ReqSend(opt->s, hdr->dest)) < 0) {
        printf("sceDeci2ReqSend fail, reason code = %08x\n", ret);
        return -1;
    }

    while (opt->rbytes == 0) {
        // rbytes is volatile; do nothing
    }
    if (opt->rbytes < 0) {
        return opt->rbytes;
    }

    hdr = (sceDeci2Hdr *)opt->rptr;
    data = opt->rptr + sizeof(sceDeci2Hdr) + sizeof(u32);
    datalen = hdr->len - (sizeof(sceDeci2Hdr) + sizeof(u32));

    if ((*(u32 *)(hdr + 1))) {
        return -1;
    }

    memcpy(result, data, datalen);
    return datalen;
}
