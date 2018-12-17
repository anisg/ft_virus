#include "crypto.h"

//=======================================================
//==================== Sort =============================

void __zone2 swap(void *a, void *b, uint64_t len){
	void *tmp = ft_malloc(len);

	ft_memcpy(tmp, a, len);
	ft_memcpy(a, b, len);
	ft_memcpy(b, tmp, len);
	ft_free(tmp);
}

void __zone2 sort(void *arr, uint64_t n, uint64_t len, int (*comp)(void *a, void *b)){
	//n^2 quick impl
	for (uint32_t i = 1; i < n; i++){
		void *a = arr + ((i-1)*len);
		void *b = arr + (i*len);
		if (comp(a, b)){
			swap(a, b, len);
			i = 0;
		}
	}
}
//=======================================================
//==================== MinQueue =========================

MinQueue __zone2 *minQueue(void *arr, uint64_t n, uint64_t len, int (*less)(void *a, void *b)){
	MinQueue *mq = ft_malloc(sizeof(*mq));
	if (!mq)
		return NULL;

	mq->arr = ft_malloc(len * n);
	if (!mq->arr)
		return NULL;
	ft_memcpy(mq->arr, arr, len*n);
	mq->n = n;
	mq->mn = n;
	mq->len = len;
	mq->less = less;
	//insert in the mq
	for (uint64_t i = 0; i < n; i++){
		mqInsert(mq, arr + (i*len));
	}
	return mq;
}

bool __zone2 mqEmpty(MinQueue *mq){
	return (mq->n == 0);
}

void __zone2 *mqExtractMin(MinQueue *mq){
	if (mqEmpty(mq))
		return NULL;
	void *tmp = ft_malloc(mq->len);
	if (!tmp)
		return NULL;
	ft_memcpy(tmp, mq->arr + 0, mq->len);
	mq->n -= 1;
	if (mq->n == 0)
		return tmp;
	ft_memcpy(mq->arr + 0, mq->arr + (mq->n * mq->len), mq->len);
	int len = mq->len;
	int i = 0;
	while (TRUE){
		int mini = i;
		int l = 2*i+1;
		int r = 2*i+2;
		if (l < mq->n && !mq->less(mq->arr + (mini * len), mq->arr + (l * len)))
			mini = l;
		if (r < mq->n && !mq->less(mq->arr + (mini * len), mq->arr + (r * len)))
			mini = r;
		if (mini != i){
			swap(mq->arr + (i * len), mq->arr + (mini * len), len);
			i = mini;
		}
		else
			break;
	}
	return tmp;
}

bool __zone2 mqInsert(MinQueue *mq, void *d){
	//O(log2(n))
	if (mq->n == mq->mn)
		return FALSE;
	int len = mq->len;
	int i = mq->n;

	ft_memcpy(mq->arr + (mq->n * len), d, len); //add at the end
	mq->n += 1;
	//order
	while (i != 0){
		void *parent = mq->arr + (((i-1)/2) * len);
		void *node = mq->arr + (i * len);
		if (mq->less(node, parent)){
			swap(node, parent, len);
			i = ((i-1)/2);
		}
		else
			break;

	}
	return TRUE;
}

//=======================================================
//==================== String ===========================

String __zone2 string(char *s, uint64_t n){
	return (String){s,n};
}
