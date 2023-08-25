
#include <iostream>
#include <math.h>
static union
{
double d;
struct
{
#ifdef LITTLE_ENDIAN
int j, i;
#else
int i, j;
#endif
 } n;
 } eco;
#define EXP_A (1048576 / M_LN2) /* use 1512775 for integer version */
#define EXP_C 60801 /* see text for choice of c values */
#define EXP(y) (eco.n.i = EXP_A*(y) + (1072693248 - EXP_C), eco.d)




// #define EXP_A 184
// #define EXP_C 16249 

// float EXP(float y){
//     union
//     {
//         float d;
//         struct{
//         #ifdef LITTLE_ENDIAN
//             short j, i;
//         #else
//             short i, j;
//         #endif
//         } n;
//     } eco;
//     eco.n.i = EXP_A*(y) + (EXP_C);
//     eco.n.j = 0;
//     return eco.d;
// }

// float LOG(float y){
//     int * nTemp = (int*)&y;
//     y = (*nTemp) >> 16;
//     return (y - EXP_C) / EXP_A;
// }

// float POW(float b, float p){
//     return EXP(LOG(b) * p);
// }


int main(){
    std::cout<<EXP_A<<std::endl;
    std::cout<<EXP(1)<<std::endl;
    std::cout<<EXP(2)<<std::endl;
    std::cout<<EXP(3)<<std::endl;
    std::cout<<EXP(4)<<std::endl;
}