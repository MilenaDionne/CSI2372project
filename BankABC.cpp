//************************************************************************
//
//   Project
//   Fall 2019
//
//   Mohamed TALEB
//
//  Authors : 
//  Mitchell Chatterjee - 8598617
//  Milena Dionne - 8916596
//  Adittya Imam - 8181736
//  Mathew George - 8248470
//
//
// This program reads information about clients and transactions in the following 2 files:
//          - clients.txt
//          - transact.txt
//
// It also allows certain operations on the read data:
//          - Display client bank accounts
//          - Updating client accounts
//
//************************************************************************
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>

#include "BankABC.h"

using namespace std;

double DepositAccount::rate = 6.50;
double totalAccounts = 0.0;

//******************************************************************
// Basic functions of the BankAccount class
//******************************************************************
inline BankAccount::BankAccount()
{
     accountId = type = updateDate = 0;
     clientName = new char[1];
     balance = 0.0;
}

inline BankAccount::BankAccount(long id, int newType,
                                char *name, long newDate, double newBalance) : accountId(id), type(newType),
                                                                               updateDate(newDate), balance(newBalance)
{
     clientName = new char[strlen(name) + 1];
     strcpy(clientName, name);
}

inline BankAccount::~BankAccount()
{
     if (clientName)
     {
          delete[] clientName;
     }
}

inline void BankAccount::setAccountId(long account)
{
     accountId = account;
}

inline void BankAccount::setType(int newType)
{
     type = newType;
}

inline void BankAccount::setClientName(char *name)
{
     if (clientName)
     {
          delete[] clientName;
     }
     clientName = new char[strlen(name) + 1];
     strcpy(clientName, name);
}

inline void BankAccount::setUpdateDate(long newDate)
{
     updateDate = newDate;
}

inline void BankAccount::setBalance(double newBalance)
{
     balance = newBalance;
}

void BankAccount::print()
{
     cout.setf(ios::fixed);
     cout.precision(2);
     cout << accountId << "\t\t\t" << type << "\t" << updateDate << "\t\t" << balance;
}

//******************************************************************
// Basic functions of the DepositAccount class
//******************************************************************
inline DepositAccount::DepositAccount(long id, int newType, char *name,
                                      long newDate, double balanceArg, int nbyear) : BankAccount(id, newType, name,
                                                                                                 newDate, balanceArg),
                                                                                     nbyears(nbyear)
{
}

inline void DepositAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

void DepositAccount::print()
{
     Bonus();
     BankAccount::print();
     cout.setf(ios::fixed);
     cout.precision(2);
     cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Functions of the class DepositAccount. It allows to calculate
// the annual bonus on deposit accounts.
// Inputs: Nothing
// Outputs: Nothing
//******************************************************************
void DepositAccount::Bonus()
{
     setBalance(getBalance() + (getBalance() * getRate() * (getNbYears()) / 36000.00));
}

//******************************************************************
// Basic functions of the LoanAccount class
//******************************************************************
inline LoanAccount::LoanAccount(long id, int newType, char *name,
                                long newDate, double newBalance, int nbyear,
                                double newRate) : BankAccount(id, newType,
                                                              name, newDate, newBalance),
                                                  nbyears(nbyear), rate(newRate)
{
}

inline void LoanAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

inline void LoanAccount::setRate(double newRate)
{
     rate = newRate;
}

void LoanAccount::print()
{
     BankAccount::print();
     cout.setf(ios::fixed);
     cout.precision(2);
     cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Basic functions of the Transaction class
//******************************************************************
inline Transaction::Transaction(long idTr, int typeTr, long dateTr,
                                int codeTr = 01, double amountTr) : accountId(idTr), type(typeTr),
                                                                    date(dateTr), code(codeTr),
                                                                    amount(amountTr)
{
}

inline void Transaction::setAccountId(long compteTr)
{
     accountId = compteTr;
}

inline void Transaction::setType(int typeTr)
{
     type = typeTr;
}

inline void Transaction::setDate(long dateTr)
{
     date = dateTr;
}

inline void Transaction::setCode(int codeTr)
{
     code = codeTr;
}

inline void Transaction::setAmount(double amountTr)
{
     amount = amountTr;
}

//****************************************************************************
// Purpose: Sort a list of bank accounts in ascending order of ids and types.
//
// WARNING: This sort leaves the last account (of id 0) of the list which has
// its position to assure the tests of end of list later !!!
//
// Inputs: listAccount(BankAccount *), a list of bank accounts.
// Outputs: listAccount(BankAccount *), sorted list of bank accounts.
//****************************************************************************
void sortAccounts(BankAccount **listAccounts)
{

     BankAccount **ptr = listAccounts;

     int flag = 1;
     BankAccount *temp;
     // Sorting ascending order
     // We need to get the id value of the double pointer listAccount and then use this code
     // Ensure that the value is not null at listAccounts[i] and listAccounts[j]
     for (int i = 0; listAccounts[i] && flag; i++)
     {
          flag = 0;
          for (int j = 0; listAccounts[j + 1]; j++)
          {
               if (listAccounts[j]->getAccountId() > listAccounts[j + 1]->getAccountId())
               {
                    temp = listAccounts[j];
                    listAccounts[j] = listAccounts[j + 1];
                    listAccounts[j + 1] = temp;
                    flag = 1;
               }
          }
     }
}

//******************************************************************
// Purpose: This function reads the file 'clients.txt' and builds
// an array containing the different bank accounts of customers.
//
// Inputs: Nothing
// Output: listAccount(type: BankAccount *), the list of bank
//         accounts read from the file 'clients.txt'.
//******************************************************************
BankAccount **readAccounts()
{
     ifstream inputFile("clients.txt"); // Opening the input file
     if (!inputFile)                    // If the file is not found...
     {
          cout << "File not found !!!" << endl;
          exit(0);
     }

     BankAccount **listAccounts = new BankAccount *[K_SizeMax];
     if (!listAccounts)
     {
          cout << "Full memory. !!!" << endl;
          return listAccounts;
     }

     BankAccount **pAccount = listAccounts;

     long accountRead, dateRead;
     int TypeRead, nbyearRead, counter = 0;
     double balanceRead, RateRead;
     char nameRead[60];

     inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
     inputFile.getline(nameRead, 60);

     while (inputFile && (counter < K_SizeMax - 1))
     {
          // YOU HAVE TO DO SOMETHING FROM HERE !!!
          // Create accounts depending on the different types 
          if (TypeRead == 03) //create term deposit account 
          {
               *pAccount = new DepositAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead); 
          }
          else if (TypeRead == 04) //create loan account 
          {
               balanceRead = balanceRead + ((balanceRead * nbyearRead * RateRead) / 36000);
               *pAccount = new LoanAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead, RateRead); //
          }
          else //create checking and saving account
          {
               *pAccount = new BankAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead);
          }
          // UNTIL THIS POINT.

          inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
          inputFile.getline(nameRead, 60);
          pAccount++;
          counter++;
     }

     // this is a sentinel value
     *pAccount = new BankAccount();
     return listAccounts;
}

//*****************************************************************************************
// Purpose: This function validates whether the transaction code
//          corresponds to the correct account:
//              - 01 ==> account (01: Check, 02: Savings, 03: Deposit and 04: Loan)
//              - 02 ==> account (01: Check, 02: Savings)
//              - 03 ==> account (01: Check).
//
// Inputs: trans (Type: Transaction) an instance of the Transaction class.
// Outputs: true (Type bool) if the transaction matches one of the accounts listed above.
// false (Type bool) otherwise.
//*****************************************************************************************
Bool BankAccount::validateTransaction(const Transaction trans) const
{
     if (((trans.getCode() == 02) && (isDepositAccount() || isLoanAccount())) ||
         ((trans.getCode() == 03) && (isDepositAccount() || isLoanAccount() || isSavingsAccount())))
     {
          return FALSE;
     }
     else
     {
          return TRUE;
     }
}

//******************************************************************************
// Purpose: This function is used to execute the transaction already performed
// (update the balance of an account).
//
// Inputs: trans (Transaction Type), instance of Transaction class
// Outputs: Nothing
//*******************************************************************************
void BankAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
     {
          if (trans.getCode() == 01) // Deposit
          {
               setBalance(getBalance() + trans.getAmount());
          }
          else
          {
               if (trans.getCode() == 02) // Withdrawal
               {
                    if (getBalance() >= trans.getAmount())
                    {
                         setBalance(getBalance() - (trans.getAmount() + 0.50));
                    }
                    else
                    {
                         cout << " insufficient balance!! " << endl;
                    }
               }
               else // Check
               {
                    if (getBalance() >= trans.getAmount())
                    {
                         setBalance(getBalance() - trans.getAmount());
                    }
                    else
                    {
                         cout << " insufficient balance!! " << endl;
                    }
               }
          }
     }
}

//***********************************************************************
// Purpose: This function is used to execute a read transaction
// (updating the balance of the loan account).
//
// Inputs: trans (Transaction Type), instance of the Transaction class
// Outputs: Nothing
//***********************************************************************
void LoanAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
     {
          if (trans.getCode() == 01) // Deposit
          {
               setBalance(getBalance() - trans.getAmount());
          }
     }
}

//*************************************************************************
// Purpose: This function allows to read the file 'transact.txt'
//          and to update the accounts concerned by the transactions read.
//
// Inputs: listAccount (type BankAccount *), the list of bank accounts.
// Output: Nothing.
//*************************************************************************

void updateAccounts(BankAccount **listAccounts)
{

     int flag = 1;
     ifstream inputFile("TRANSACT.txt"); // Opening the input file
     if (!inputFile)                     // If the file is not found...
     {
          cout << "File not found !!!" << endl;
          exit(0);
     }

     long accountRead, dateRead;
     int TypeRead, nbyearRead, transactionRead, counter = 0;
     double balanceRead;

     inputFile >> accountRead >> TypeRead >> dateRead >> transactionRead >> balanceRead;

     while (inputFile && (counter < K_SizeMax - 1))
     {
          int flag = 1;
          // checks for the account that matches the update file
          for (int i = 0; listAccounts[i] && flag; i++)
          {
               BankAccount *currentVal = listAccounts[i];
               if (currentVal->getAccountId() == accountRead && currentVal->getType() == TypeRead)
               {

                    if (currentVal->isLoanAccount())
                    {
                         // can only do transaction type 1 (deposit) on loan accounts
                         if (transactionRead != 1)
                         {
                              flag = 0;
                              continue;
                         }
                         currentVal->setBalance(currentVal->getBalance() - balanceRead);
                    }
                    else if (currentVal->isDepositAccount())
                    {
                         // can only do transaction type 1 (deposit) on term accounts
                         if (transactionRead != 1)
                         {
                              flag = 0;
                              continue;
                         }
                         currentVal->setBalance(currentVal->getBalance() + balanceRead);
                    }
                    else if (currentVal->isCheckingAccount())
                    {
                         // cannot do transaction type 3 (check) on savings accounts
                         if (transactionRead == 3)
                         {
                              flag = 0;
                              continue;
                         }
                         else if (transactionRead == 2)
                         { // withdrawl
                              currentVal->setBalance(currentVal->getBalance() - balanceRead);
                         }
                         else if (transactionRead == 1)
                         { // deposit
                              currentVal->setBalance(currentVal->getBalance() + balanceRead);
                         }
                         // remove $0.50
                         currentVal->setBalance(currentVal->getBalance() - 0.50);
                    }
                    else if (currentVal->isSavingsAccount())
                    {
                         if (transactionRead == 1)
                         { // deposit
                              currentVal->setBalance(currentVal->getBalance() + balanceRead);
                         }
                         else if (transactionRead == 2)
                         { // withdrawl
                              currentVal->setBalance(currentVal->getBalance() - balanceRead);
                         }
                         else if (transactionRead == 3)
                         { // check
                              currentVal->setBalance(currentVal->getBalance() - balanceRead);
                         }
                         // remove $0.50
                         currentVal->setBalance(currentVal->getBalance() - 0.50);
                    }
               }
          }
          inputFile >> accountRead >> TypeRead >> dateRead >> transactionRead >> balanceRead;
          counter++;
     }
}

BankAccount *runDynamicCast(BankAccount *currentVal)
{ //help method to implement updateAccounts
     if (currentVal->isLoanAccount())
     {
          return dynamic_cast<LoanAccount *>(currentVal);
     }
     else if (currentVal->isDepositAccount())
     {
          return dynamic_cast<DepositAccount *>(currentVal);
     }

     return dynamic_cast<BankAccount *>(currentVal);
}

//******************************************************************************
// Purpose: This function displays the list of bank accounts for all customers.
//
// Inputs: listAccount (type: BankAccount *), the list of bank accounts.
// Outputs: Nothing
//******************************************************************************
void displayAccounts(BankAccount **listAccounts)
{
     cout << endl
          << endl
          << endl;

     Bool find[K_SizeMax];
     for (int k = 0; k < K_SizeMax; k++)
     {
          find[k] = FALSE;
     }

     cout << "                       THE REPORT OF THE BANK ACCOUNTS OF CLIENTS" << endl;
     cout << "                       ------------------------------------------" << endl
          << endl;
     int i = 0;

     for (int i = 1; listAccounts[i]; i++)
     {
          BankAccount *currentVal = runDynamicCast(listAccounts[i]);

          cout << "Client Name: " << currentVal->getClientName() << endl
               << endl;
          cout << "Bank Account"
               << "\t\t"
               << "Type"
               << "\t"
               << "Update Date"
               << "\t"
               << "Balance"
               << "\t\t"
               << "Nb.Years"
               << "\t"
               << "Rate" << endl;
          cout << "-------------"
               << "\t\t"
               << "------"
               << "\t"
               << "-----------"
               << "\t"
               << "-------"
               << "\t\t"
               << "-----------"
               << "\t"
               << "------" << endl;
          currentVal->print();
          cout << endl;

          // must update this after printing for the case of deposit accounts updating their bonus
          double totalAccountValue = currentVal->getBalance();

          // This loop is meant to print out the subsequent elements of one client
          for (int j = i + 1; listAccounts[j]; j++)
          {
               if (listAccounts[i]->getAccountId() != listAccounts[j]->getAccountId())
               {
                    break;
               }
               currentVal = runDynamicCast(listAccounts[j]);

               // Regular print all the values
               currentVal->print();
               cout << endl;

               // must update this after printing for the case of deposit accounts updating their bonus
               totalAccountValue += currentVal->getBalance();
               i++;
          }
          cout << "\t\t\t\t\t        -------------" << endl;
          cout << "\tTOTAL ACCOUNTS: " << totalAccountValue << endl
               << endl
               << endl;
     }
     cout << " THE REPORT OF THE BANK ACCOUNTS OF CLIENTS" << endl;
}

int main()
{
     BankAccount **list = readAccounts();

     sortAccounts(list);
     displayAccounts(list);
     updateAccounts(list);
     cout << endl
          << endl;
     cout << "               ************************************************" << endl;
     cout << "               * REAFFICHAGE DES DONNEES APRES LA MISE A JOUR *" << endl;
     cout << "               ************************************************" << endl;
     displayAccounts(list);
     cout << endl;

     system("PAUSE");
     return 0;
}
