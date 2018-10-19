#include "crypto.h"

//=======================================================
//==================== Sort =============================

void __swap(void *a, void *b, uint64_t len){
	void *tmp = (len);

	ft_memcpy(tmp, a, len);
	ft_memcpy(a, b, len);
	ft_memcpy(b, tmp, len);
	ft_free(tmp);
}

void sort(void *arr, uint64_t n, uint64_t len, int (*comp)(void *a, void *b)){
	//n^2 quick impl
	for (int i = 1; i < n; i++){
		void *a = arr + ((i-1)*len);
		void *b = arr + (i*len);
		if (comp(a, b)){
			__swap(a, b, len);
			i = 0;
		}
	}
}
//=======================================================
//==================== MinQueue =========================

MinQueue *minQueue(void *arr, uint64_t n, uint64_t len, int (*less)(void *a, void *b)){
	MinQueue *mq = ft_malloc(sizeof(*mq));

	mq->arr = ft_malloc(len * n);
	ft_memcpy(mq->arr, arr, len*n);
	mq->n = n;
	mq->mn = n;
	mq->len = len;
	mq->less = less;
	sort(arr, n, len, less);
	return mq;
}

bool mqEmpty(MinQueue *mq){
	return (mq->n == 0);
}

void *mqExtractMin(MinQueue *mq){
	if (mqEmpty(mq))
		return NULL;
	void *tmp = ft_malloc(mq->len);
	ft_memcpy(tmp, mq->arr + ((mq->n-1) * mq->len), mq->len);
	mq->n -= 1;
	return tmp;
}

bool mqInsert(MinQueue *mq, void *d){
	//n^2 insert LOL
	if (mq->n == mq->mn)
		return FALSE;
	ft_memcpy(mq->arr + (mq->n * mq->len), d, mq->len);
	mq->n += 1;
	sort(mq->arr, mq->n, mq->len, mq->less);
	return TRUE;
}


//=======================================================
//==================== String ===========================

String *string(char *s, uint64_t n){
	String *st = ft_malloc(sizeof(String));
	if (!st)
		return NULL;
	st->s = s;
	st->n = n;
	return st;
}
