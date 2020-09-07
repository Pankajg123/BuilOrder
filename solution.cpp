// C++11 Standard. Assumption: input file named "inputBO.txt" is kept in same folder with the program.  
// compile & run command: cd "<path for program>" &&  && g++ solution.cpp -o solution && "<path for program>\"solution
#include <iostream>
#include <fstream>
#include <string> 
#include<map>
#include<vector>
#include <sstream>
#include <stack>
#include <unordered_set>
using namespace std;
void visitProcesstUtil(map<string,vector<string>>&mapOfBuildPackages,string& package, stack<string>&processedBufferStack,
            unordered_set<string>&vistedBufferSet,unordered_set<string>&processedBufferSet);

void printResult(stack<string>&finalBufferStack); 

int main()
{
   vector<string>vecOfbuildSource;
   map<string, vector<string>>mapOfBuildPackages;
   unordered_set<string>setOfAllBuildPackages;
   fstream inputfile;
   inputfile.open("inputBO.txt",ios::in); //open a file to perform read operation using file object
   if (inputfile.is_open()){   //checking whether the file is open
      string tp;
      //first read first line to get the count of build packages
      getline(inputfile, tp);
      int buildPackagesSourcesCount = stoi(tp);
      //cout<<buildPackagesSourcesCount<<endl;
      while(getline(inputfile, tp))
      { //read data from file object and put it into string.
         if(tp.size()>0)
         {
             istringstream iss(tp);
             string word;
             string key;
             vector<string>vectorOfDependencies;
             iss >> word;
             key = word;
             setOfAllBuildPackages.insert(key);
             vecOfbuildSource.push_back(key);
             while (iss >> word)
             {
             vectorOfDependencies.push_back(word);
             setOfAllBuildPackages.insert(word);
             }
             mapOfBuildPackages[key]=vectorOfDependencies;
         }
      }
      inputfile.close(); //close the file object.
   }

   stack<string>processedBufferStack,finalBufferStack;
   unordered_set<string> vistedBufferSet, processedBufferSet;

    for (string package : setOfAllBuildPackages) 
      {
            if (vistedBufferSet.find(package)!=vistedBufferSet.end()) {  //TC : O(1)
                continue;
            }
            visitProcesstUtil(mapOfBuildPackages,package,processedBufferStack,vistedBufferSet,processedBufferSet);
      }  

      while(!processedBufferStack.empty())
      {
         finalBufferStack.push(processedBufferStack.top());
         processedBufferStack.pop();
      }

      printResult(finalBufferStack);
      return 0;
}    


void visitProcesstUtil(map<string, vector<string>>&mapOfBuildPackages, string& package, stack<string>&processedBufferStack,
      unordered_set<string>&vistedBufferSet,unordered_set<string>&processedBufferSet) // Time Comp :O(n), Space Comp: O(n)
{
   try{
      vistedBufferSet.insert(package); 
      auto mItr = mapOfBuildPackages.find(package); 
      if(mItr != mapOfBuildPackages.end()) 
      {   //value part of map is a vector of dependency strings. iterarating & processed the vector content
            for(auto vitr =(mItr->second).begin();vitr!=(mItr->second).end(); ++vitr)
            {
               if (vistedBufferSet.find(*vitr)!=vistedBufferSet.end()) //TC : O(1)
               {
                  if(processedBufferSet.find(*vitr)!=processedBufferSet.end()) //TC : O(1)
                  {
                     continue;
                  }
                  else
                  {
                     throw "Build Order is not Valid due to cyclic dependency.";
                  }
               }
                  visitProcesstUtil(mapOfBuildPackages,*vitr,processedBufferStack,vistedBufferSet,processedBufferSet);
            }
      }
      processedBufferStack.push(package);
      processedBufferSet.insert(package);
   }
   catch (const char* msg) {
     cerr << msg << endl;
     exit(0);
   }


}

void printResult(stack<string>&finalBufferStack) //Time Complexity= O(n), Space Complexity = O(1)
{
   cout<<"Build Order:"<<endl;
   while(!finalBufferStack.empty())  
   {
      cout<< finalBufferStack.top()<<" "; 
      finalBufferStack.pop();             
   }
}


