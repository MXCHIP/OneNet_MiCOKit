#ifndef __REGINA_COMMON_H__
#define __REGINA_COMMON_H__


#ifdef __cplusplus
extern "C" {
#endif


/*
 * Standard error handling macros
 */
#define REGINA_RETURN_IF_ERR(E) \
        { if (E) return E; }

#define REGINA_RETURN_IF_NIL(p, E) \
        { if (!(p)) return E; }

#define REGINA_RETURN_IF_NIL2(p1, p2, E) \
        { if (!(p1) || !(p2)) return E; }

#define REGINA_RETURN_IF_NIL3(p1, p2, p3, E) \
        { if (!(p1) || !(p2) || !(p3)) return E; }

#define REGINA_RETURN_IF_NIL4(p1, p2, p3, p4, E) \
        { if (!(p1) || !(p2) || !(p3) || !(p4)) return E; }

#define REGINA_RETURN_IF_NIL5(p1, p2, p3, p4, p5,E) \
        { if (!(p1) || !(p2) || !(p3) || !(p4) || !(p5)) return E; }

#include "regina/error.h"

typedef enum {
    SERVER_BOOTSTRAP,
    SERVER_ACCEPTOR
} server_type_t;


#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif /* __REIGNA_COMMON_H__ */
