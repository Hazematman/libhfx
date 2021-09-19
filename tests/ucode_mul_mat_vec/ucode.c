extern void mul_mat_vec(void);
extern void setup_data(void);
extern void output_data(void);

int main()
{
    setup_data();
    mul_mat_vec();
    output_data();
    asm volatile ("break");
    return 0;
}
