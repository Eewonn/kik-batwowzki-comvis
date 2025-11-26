#include<iostream>
using namespace std;

void computeAns(double *num, double *top, double *ans){
    *ans = 1;
    for (int c = 1; c <= int(*top); c++)
        *ans = *ans * *num;
}

int main(){
 double num, top, result;

 cout<<"Enter number 1: ";

 cin>>num;  //input value 5
 cout<<"Enter number 2: ";
 cin>>top;  //input value 4
 
 computeAns(&num,&top, &result);
 cout << "Result: " << result << endl;

return 0;
}
