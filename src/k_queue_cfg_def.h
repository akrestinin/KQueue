#ifndef K_QUEUE_CFG_H
#define K_QUEUE_CFG_H

#include <stdlib.h>
#include <string.h>

#ifndef K_QUEUE_MALLOC_CUSTOM
#ifndef K_QUEUE_MALLOC
#define K_QUEUE_MALLOC(sz) malloc((sz))
#endif /* K_QUEUE_MALLOC */
#else
/* User custom code here */
#endif /* K_QUEUE_MALLOC_CUSTOM */

#ifndef K_QUEUE_FREE_CUSTOM
#ifndef K_QUEUE_FREE
#define K_QUEUE_FREE(pM) free((pM))
#endif /* K_QUEUE_FREE */
#else
/* User custom code here */
#endif /* K_QUEUE_FREE_CUSTOM */

#ifndef K_QUEUE_MEMCPY_CUSTOM
#ifndef K_QUEUE_MEMCPY
#define K_QUEUE_MEMCPY(pD, pS, sz) memcpy((pD), (pS), (sz))
#endif /* K_QUEUE_MEMCPY */
#else
/* User custom code here */
#endif /* K_QUEUE_MEMCPY_CUSTOM */

#ifndef K_QUEUE_MEMSET_CUSTOM
#ifndef K_QUEUE_MEMSET
#define K_QUEUE_MEMSET(pD, v, sz) memset((pD), (v), (sz))
#endif /* K_QUEUE_MEMSET */
#else
/* User custom code here */
#endif /* K_QUEUE_MEMCPY_CUSTOM */

#endif /* K_QUEUE_CFG_H */
