#include <iostream>
#include <cmath>
using namespace std ;

int BinaryToInteger(string binary, int len){
    // given a string of bits and it's length, it returns it's integer value, assuming 2's complement system
    int power = 1; int ans = 0 ;
    int bit , value ;
    
    int number = stoi(binary) ;
    for(int i = 0; i < len; i++){
        bit = number % 10 ;
        value = bit * power ;
        ans = ans + value ;
        
        power = 2 * power ;
        number = number / 10 ;
    }
    
    if(binary[0] == '1'){
        // power == pow(2, len) ;
        ans = power - ans ;
        ans = - ans ;
    }
    
    return ans ;
}

int main(){
    string b; int l ;
    cout << "What's the binary? " ; cin >> b ;
    cout << "What's it's length? " ; cin >> l ;  
    cout << "\nIt's value is: " << BinaryToInteger(b, l) ;
    return 0 ; 
}
