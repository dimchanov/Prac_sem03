
STYPE 
bit_reverse(STYPE value){
    UTYPE u_num = (UTYPE) value;
    UTYPE ans = 0;

    int bit_size = sizeof(ans) * 8;
    while(u_num != 0) {
        ans = ans | ((u_num & 1) << (bit_size - 1));
        --bit_size;
        u_num >>= 1;
    }

    return ans;
}
