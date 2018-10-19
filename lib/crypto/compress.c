#include "crypto.h"

//=======================================================
//==================== Bit Array ========================

bool getbit(char *a, uint64_t p){
	return (a[p/8] & (1 << (p%8))) != 0;
}

void setbit(char *a, uint64_t p){
	a[p/8] |= (1 << (p%8));
}

void clearbit(char *a, uint64_t p){
	a[p/8] &= ~(1 << (p%8));
}

//=======================================================
//==================== Sorting Node =====================

int comp(void *a, void *b){
	Node *na = (Node*)a;
	Node *nb = (Node*)b;
	return na->freq < nb->freq;
}

//=======================================================
//============== Table/Arr Of Nodes =====================

int tableToArr(Node *t, int n){
	sort(t, n, sizeof(Node), comp);
	int i;
	for (i = 0; i < n; i++)
		if (t[i].freq == 0)
			return i;
	return i;
}

//=======================================================
//==================== Huffman ==========================

Node *huffmanTree(Node *arr, int n){
	MinQueue *mq = minQueue(arr, n, sizeof(Node), comp);

	for (int i = 0; i < n-1; i++){
		Node *a = mqExtractMin(mq);
		Node *b = mqExtractMin(mq);
		Node z = (Node){NOP, a->freq + b->freq, a, b};
		mqInsert(mq, &z);
	}
	return mqExtractMin(mq); //root of the tree
}

uint32_t __huffmanCost(Node *node, int d){
	if (!node)
		return 0;
	if (node->l == NULL && node->r == NULL){
		return node->freq * d;
	}
	return __huffmanCost(node->l, d+1) + __huffmanCost(node->r, d+1);
}

uint32_t huffmanCost(Node *r){
	return __huffmanCost(r, 0);
}

//=======================================================
//==================== Huffman++ ========================

void __huffmanSetCodeTable(Node *node, CharCode *codes, int d, uint64_t rep){
	if (node == NULL)
		return ;
	if (node->v != NOP){
		codes[node->v] = (CharCode){rep, d};
	}
	__huffmanSetCodeTable(node->l, codes, d+1, ((rep << 1) | 0));
	__huffmanSetCodeTable(node->r, codes, d+1, ((rep << 1) | 1));
}

void huffmanSetCodeTable(Node *r, CharCode *codes){
	__huffmanSetCodeTable(r, codes, 0, 0);
}

bool __huffmanParse(Node *node, char *s, uint64_t n, uint64_t *bp, char *c){
	if (node == NULL)
		return FALSE;
	if (node->v != NOP){
		(*c) = node->v;
		return TRUE;
	}
	int b = getbit(s, *bp);
	(*bp) += 1;
	if (b == 0)
		return __huffmanParse(node->l, s, n, bp, c);
	else
		return __huffmanParse(node->r, s, n, bp, c);
}

bool huffmanParse(Node *r, char *s, uint64_t n, uint64_t *bp, char *c){
	return __huffmanParse(r, s, n, bp, c);
}

void huffmanDelete(Node *node){
	if (!node)
		return ;
	huffmanDelete(node->l);
	huffmanDelete(node->r);
	ft_free(node);
}
//=======================================================
//=============== Header formatting =====================

void setEncodedNode(char *s, int nsize, int v, uint32_t freq){
	switch (nsize){
		case sizeof(EncodedNode8): ((EncodedNode8*)s)[0] = (EncodedNode8){v, freq}; break;
		case sizeof(EncodedNode16): ((EncodedNode16*)s)[0] = (EncodedNode16){v, freq}; break;
		case sizeof(EncodedNode32): ((EncodedNode32*)s)[0] = (EncodedNode32){v, freq}; break;
	}
}

EncodedNode getEncodedNode(char *s, int nsize){
	int v;
	uint32_t freq;
	switch (nsize){
		case sizeof(EncodedNode8): v = ((EncodedNode8*)s)[0].v; freq = (uint8_t)(((EncodedNode8*)s)[0].freq); break;
		case sizeof(EncodedNode16): v = ((EncodedNode16*)s)[0].v; freq = (uint16_t)(((EncodedNode16*)s)[0].freq); break;
		case sizeof(EncodedNode32): v = ((EncodedNode32*)s)[0].v; freq = (uint32_t)(((EncodedNode32*)s)[0].freq); break;
	}
	return (EncodedNode){v, freq};
}

//=======================================================
//=============== Compress Algorithm ====================


void __add(unsigned char c, CharCode *codes, char *s, uint64_t *bp){
	for (int i = 0; i < codes[c].n; i++){
		if ((codes[c].rep & (1 << (codes[c].n-i-1))) != 0)
			setbit(s,(*bp)+i);
		else
			clearbit(s,(*bp)+i);
	}
	(*bp) += codes[c].n;
}

String *compress(String *b){
	//==== Count frequencies ====
	Node t[256];
	for (int i = 0; i < 256; i++){
		t[i] = (Node){i, 0, NULL, NULL};
	}
	for (int i = 0; i < b->n; i++)
		t[(unsigned char)b->s[i]].freq += 1;
	int n = tableToArr(t, 256);
	uint32_t maxfreq = 0;
	for (int i = 0; i < n; i++)
		maxfreq = max(maxfreq, t[i].freq);
	//==== huffman ====
	Node *r = huffmanTree(t, n);
	//==== get compressed size ====
	int nsize = (maxfreq <= 255) ? sizeof(EncodedNode8) : (maxfreq <= 65535) ? sizeof(EncodedNode16) : sizeof(EncodedNode32);
	int starthsize = sizeof(char) * 2;
	int hsize = (n * nsize) + starthsize; //header size
	int cost = huffmanCost(r);                                  //cost in bits (i.e: 1char = 8bits)
	uint32_t size = ((cost+(8-cost%8))/8) + hsize;
	char *s = ft_malloc(size);
	//==== encode header ====
	s[0] = nsize;                                               //0 to 1: node size (between [1, 2, 4])
	s[1] = n;                                    				//1 to 2: number of elements
	for (int i = 0; i < n; i+=1){                               //3 to x: array of (freq/value)
		setEncodedNode(s + starthsize + (i * nsize), nsize, t[i].v, t[i].freq);
	}
	//==== encode chars ====
	CharCode codes[256]; //table
	huffmanSetCodeTable(r, codes);
	uint64_t bp = 0;
	for (int i = 0; i < b->n; i++){
		__add(b->s[i], codes, s+hsize, &bp);
	}
	//==== STAT ====
	//printf("header:%d, compressed:%d, total:%d >> %.1f%% of original (%+d bytes)\n", hsize, ((cost+(8-cost%8))/8), size, ((float)size)/((float)b->n)*100.0,((int64_t)size)-((int64_t)b->n) );
	//printf("max depth:%d, nb of characters:%d\n", md, n);
	huffmanDelete(r);
	return string(s, size);
}

//=======================================================
//=============== Decompress Algorithm ==================

String *decompress(String *b){
	Node t[256];
	//==== Parse header (and get decompressed size) ====
	unsigned char nsize = b->s[0];
	unsigned char n = b->s[1];
	int starthsize = sizeof(char) * 2;
	int hsize = (n * nsize) + starthsize;
	uint64_t size = 0;
	for (int i = 0; i < n; i++){
		EncodedNode node = getEncodedNode(b->s + starthsize + (i * nsize), nsize);
		t[i] = (Node){node.v, node.freq, NULL, NULL};
		size += node.freq;
	}
	//==== huffman ====
	Node *r = huffmanTree(t, n);
	//==== Parse encoded chars ====
	char *s = ft_malloc(size);
	char c;
	uint64_t cost = huffmanCost(r);
	uint64_t bp = 0;
	int i = 0;
	while (bp < cost){
		if (huffmanParse(r, b->s + hsize, b->n, &bp, &c))
			s[i++] = c;
	}
	huffmanDelete(r);
	return string(s, size);
}

/*
//=======================================================
//=============== Test Main =============================

int main(int ac, char **av){
	char *s; size_t l;
	char cmpr[1024]; char dcmpr[1024];

	if (!fget(av[1], &s, &l))
		return -1;
	sprintf(cmpr, "%s.cmpr", av[1]);
	sprintf(dcmpr, "%s.dcmpr", av[1]);
	String *in = string(s,l);

	printf("Compress\n");
	String *out = compress(in);

	printf("Decompress\n");
	String *test = decompress(out);

	if (!fput(dcmpr, test->s, test->n) || !fput(cmpr, out->s, out->n))
		printf("FAIL\n");
	else
		printf("Done! created \'%s\' (compressed file) and \'%s\' (decompressed file)\n", cmpr, dcmpr);
}
*/
