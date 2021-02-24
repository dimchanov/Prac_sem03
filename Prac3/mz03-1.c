int bitcount(STYPE value)
{
    UTYPE u_val = (UTYPE) value;
    int count_bit = 0;
    while (u_val) {
        count_bit += u_val & 1;
        u_val >>= 1;
    }
    return count_bit;
}
