void insert(char **s1, uint64_t *n1, uint64_t pos, char *s2, uint64_t n2){
        char *ns;
        if ((ns = (char *)malloc((*n1) + n2)) == NULL) return FALSE;
        uint64_t i,j,l;
        for (i = 0; i <= pos; i += 1){ ns[i] = (*s1)[i]; }
        for (j = 0; j < n2; j += 1){ns[i+j] = s2[j]; }
        for (l = 0; i+l < *n1; l += 1){ ns[i+j+l] = (*s1)[i+l]; }
        clean(*s1);
        *s1 = ns;
        *n1 = (*n1) + n2;
        return TRUE;
}
