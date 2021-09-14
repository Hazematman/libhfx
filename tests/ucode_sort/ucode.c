extern void draw_tri(void);
extern void setup_data(void);
extern void output_data(void);

int main()
{
    setup_data();
    sort_tri();
    output_data();
    asm volatile ("break");
    return 0;
}
