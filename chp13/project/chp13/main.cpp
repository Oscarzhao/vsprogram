// use brass class -- testing bank account classes
// compile with Brass.cpp
#include <iostream>
//#include "brass.h"
#include "acctabc.h"

int main(){
    using std::cout;
    using std::endl;

    Brass Piggy("Porcelot Pigg", 381299, 4000.0);  
    BrassPlus Hoggy("Horatio Hogg", 382288, 3000.0);
    Piggy.ViewAcct();
    cout << endl;
    Hoggy.ViewAcct();
    cout << endl;

    cout << "Depositing $1000 into the Hoggy Account: \n";
    Hoggy.Deposit(1000.0);
    cout << "New balance is: $" << Hoggy.Balance() << endl;
    cout << "Withdrawing #4200 from the Piggy Account: \n";
    Piggy.Withdraw(4200.0);
    cout << "Piggy account balance: $" << Piggy.Balance() << endl;
    cout << "WithDrawing $4200 from the Hogg Account: \n";
    Hoggy.Withdraw(4200.0);
    Hoggy.ViewAcct();


    system("pause");
    return 0;
}