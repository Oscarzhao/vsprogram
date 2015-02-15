// brass.cpp  -- bank account class methods
#include <iostream>
#include <cstring>
#include "brass.h"
using std::cout;
using std::ios_base;
using std::endl;
// Brass methods
Brass::Brass(const char * s, long an, double bal){
    strncpy(fullname, s, Brass::MAX-1);
    fullname[Brass::MAX-1] = '\0';
    acctNum = an;
    balance = bal;
}
void Brass::Deposit(double amt){
        balance += amt;
}
void Brass::Withdraw(double amt){
    if(amt > balance)
        cout << "Not enough money to withdraw\n";
    else
        balance -= amt;
}
double Brass::Balance() const{
    return balance;
}

void Brass::ViewAcct() const{
    /// set up ###.## format
    ios_base::fmtflags initialState = cout.setf(ios_base::fixed, ios_base::floatfield);
    cout.setf(ios_base::showpoint);
    cout.precision(2);
    cout << "Client: " << fullname << endl;
    cout << "Account number: " << acctNum << endl;
    cout << "Balance: $" << balance << endl;
    cout.setf(initialState);   // restore original format
}

// BrassPlus method
BrassPlus::BrassPlus(const char * s, long an, double bal, double ml, double r):Brass(s, an, bal){
    maxLoan = ml;
    owesBank = 0.0;
    rate = r;
}

BrassPlus::BrassPlus(const Brass & ba, double ml, double r):Brass(ba){
    maxLoan = ml;
    owesBank = 0.0;
    rate = r;
}

void BrassPlus::ViewAcct() const{
    ios_base::fmtflags inititalState = cout.setf(ios_base::fixed, ios_base::floatfield);
    cout.setf(ios_base::showpoint);
    cout.precision(2);
    
    Brass::ViewAcct();
    cout << "Maximum Loan: $" << maxLoan << endl;
    cout << "Owed to bank: $" << owesBank << endl;
    cout << "Loan rate: " << 100 * rate << "%\n";
    cout << cout.setf(inititalState);
}

// redefine how withdraw works
void BrassPlus::Withdraw(double amt){
    // set up ###.## format
    ios_base::fmtflags initialState = cout.setf(ios_base::fixed, ios_base::floatfield);
    cout.setf(ios_base::showpoint);
    cout.precision(2);
    double bal = Balance();

    if(amt <= bal)
        Brass::Withdraw(amt);
    else if(amt <= bal + maxLoan - owesBank){
        double advance = amt - bal;
        owesBank += advance * (1.0 + rate);
        cout << "Bank advance: $" << advance << endl;
        cout << "Finance charge: $" << advance * rate << endl;
        Deposit(advance);
        Brass::Withdraw(amt);
    }else
        cout << "Credit limit exceeded. Transaction cancelld.\n";
    cout.setf(initialState);
}
