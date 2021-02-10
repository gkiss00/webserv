unsigned int width(int n) {
    unsigned int tot = 1;

    if (n < 0)
        tot++;
    while ((n /= 10))
        tot++;
    return tot;
}
