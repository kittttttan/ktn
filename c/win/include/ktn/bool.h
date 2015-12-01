#ifndef CKTN_BOOL_H_
#define CKTN_BOOL_H_

#ifndef __cplusplus

#ifndef _Bool
#define _Bool  unsigned char
#endif /* _Bool */

typedef _Bool bool;
#define false   0
#define true    1

#endif /* __cplusplus */

#endif /* CKTN_BOOL_H_ */