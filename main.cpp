#include <bits/stdc++.h>
using namespace std;

class bank{
    public:
    string name;
    int total_amount;
    set<string> types;
};

int getMinIndex(bank net_amounts[],int number_of_banks){
    int min=INT_MAX, minIndex=-1;
    for(int i=0;i<number_of_banks;i++){
        if(net_amounts[i].total_amount == 0) continue;
        
        if(net_amounts[i].total_amount < min){
            minIndex = i;
            min = net_amounts[i].total_amount;
        }
    }
    return minIndex;
}

int getSimpleMaxIndex(bank net_amounts[],int number_of_banks){
    int max=INT_MIN, maxIndex=-1;
    for(int i=0;i<number_of_banks;i++){
        if(net_amounts[i].total_amount == 0) continue;
        
        if(net_amounts[i].total_amount > max){
            maxIndex = i;
            max = net_amounts[i].total_amount;
        }
    }
    return maxIndex;
}

pair<int,string> getMaxIndex(bank net_amounts[],int number_of_banks,int minIndex,bank input[],int maxNumTypes){
    int max=INT_MIN;
    int maxIndex=-1;
    string matchingType;
    
    for(int i=0;i<number_of_banks;i++){
        if(net_amounts[i].total_amount == 0) continue;
        
        if(net_amounts[i].total_amount < 0) continue;
        
        
        vector<string> v(maxNumTypes);
        vector<string>::iterator ls=set_intersection(net_amounts[minIndex].types.begin(),net_amounts[minIndex].types.end(), net_amounts[i].types.begin(),net_amounts[i].types.end(), v.begin());
        
        if((ls-v.begin())!=0 && max<net_amounts[i].total_amount ){
            max=net_amounts[i].total_amount;
            maxIndex=i;
            matchingType = *(v.begin());
        } 
    }
    
    return make_pair(maxIndex,matchingType);
}

void printAns(vector<vector<pair<int,string>>> ans_graph, int number_of_banks,bank input[]){
    
    cout<<"\nThe transactions for minimum cash flow are as follows : \n\n";
    for(int i=0;i<number_of_banks;i++){
        for(int j=0;j<number_of_banks;j++){
            
            if(i==j) continue;
            
            if(ans_graph[i][j].first != 0 && ans_graph[j][i].first != 0){
                
                if(ans_graph[i][j].first == ans_graph[j][i].first){
                    ans_graph[i][j].first=0;
                    ans_graph[j][i].first=0;
                }
                else if(ans_graph[i][j].first > ans_graph[j][i].first){
                    ans_graph[i][j].first -= ans_graph[j][i].first; 
                    ans_graph[j][i].first =0;
                    
                    cout<<input[i].name<<" should pay Rs" << ans_graph[i][j].first<< "to "<<input[j].name<<" via "<<ans_graph[i][j].second<<endl;
                }
                else{
                    ans_graph[j][i].first -= ans_graph[i][j].first;
                    ans_graph[i][j].first = 0;
                    
                    cout<<input[j].name<<" should pay Rs "<< ans_graph[j][i].first<<" to "<<input[i].name<<" via "<<ans_graph[j][i].second<<endl;
                    
                }
            }
            else if(ans_graph[i][j].first != 0){
                cout<<input[i].name<<" should pay Rs "<<ans_graph[i][j].first<<" to "<<input[j].name<<" via "<<ans_graph[i][j].second<<endl;
                
            }
            else if(ans_graph[j][i].first != 0){
                cout<<input[j].name<<" should pay Rs "<<ans_graph[j][i].first<<" to "<<input[i].name<<" via "<<ans_graph[j][i].second<<endl;
                
            }
            
            ans_graph[i][j].first = 0;
            ans_graph[j][i].first = 0;
        }
    }
    cout<<"\n";
}

void minimizeCashFlow(int number_of_banks,bank input[],unordered_map<string,int>& indexOf,int numTransactions,vector<vector<int>>& graph,int maxNumTypes){
    
    bank net_amounts[number_of_banks];
    
    for(int b=0;b<number_of_banks;b++){
        net_amounts[b].name = input[b].name;
        net_amounts[b].types = input[b].types;
        
        int amount = 0;
        for(int i=0;i<number_of_banks;i++){
            amount += (graph[i][b]);
        }
        
        for(int j=0;j<number_of_banks;j++){
            amount += ((-1) * graph[b][j]);
        }
        
        net_amounts[b].total_amount = amount;
    }
    
    vector<vector<pair<int,string>>> ans_graph(number_of_banks,vector<pair<int,string>>(number_of_banks,{0,""}));
    
    
    int numZerototal_amounts=0;
    
    for(int i=0;i<number_of_banks;i++){
        if(net_amounts[i].total_amount == 0) numZerototal_amounts++;
    }
    while(numZerototal_amounts!=number_of_banks){
        
        int minIndex=getMinIndex(net_amounts, number_of_banks);
        pair<int,string> maxAns = getMaxIndex(net_amounts, number_of_banks, minIndex,input,maxNumTypes);
        
        int maxIndex = maxAns.first;
        
        if(maxIndex == -1){
            
            (ans_graph[minIndex][0].first) += abs(net_amounts[minIndex].total_amount);
            (ans_graph[minIndex][0].second) = *(input[minIndex].types.begin());
            
            int simpleMaxIndex = getSimpleMaxIndex(net_amounts, number_of_banks);
            (ans_graph[0][simpleMaxIndex].first) += abs(net_amounts[minIndex].total_amount);
            (ans_graph[0][simpleMaxIndex].second) = *(input[simpleMaxIndex].types.begin());
            
            net_amounts[simpleMaxIndex].total_amount += net_amounts[minIndex].total_amount;
            net_amounts[minIndex].total_amount = 0;
            
            if(net_amounts[minIndex].total_amount == 0) numZerototal_amounts++;
            
            if(net_amounts[simpleMaxIndex].total_amount == 0) numZerototal_amounts++;
            
        }
        else{
            int transactionAmount = min(abs(net_amounts[minIndex].total_amount), net_amounts[maxIndex].total_amount);
            
            (ans_graph[minIndex][maxIndex].first) += (transactionAmount);
            (ans_graph[minIndex][maxIndex].second) = maxAns.second;
            
            net_amounts[minIndex].total_amount += transactionAmount;
            net_amounts[maxIndex].total_amount -= transactionAmount;
            
            if(net_amounts[minIndex].total_amount == 0) numZerototal_amounts++;
            
            if(net_amounts[maxIndex].total_amount == 0) numZerototal_amounts++;
        }
        
    }
    
    printAns(ans_graph,number_of_banks,input);
}

int main() 
{ 
    
    cout<<"The number of transactions between various banks throughout the globe using various payment methods is reduced by this approach.A single global bank that accepts all payment methods serves as a bridge between banks that do not share a common payment method. \n\n";
    cout<<"Enter the number of banks between which the money exchange is to happen.\n";
    int number_of_banks;cin>>number_of_banks;
    
    bank input[number_of_banks];
    unordered_map<string,int> indexOf;
    
    cout<<"Enter the following details:\n";
    cout<<"Bank name ,number of payment modes and the payment modes.\n";
    
    int maxNumTypes;
    for(int i=0; i<number_of_banks;i++){
        if(i==0){
            cout<<"Central Bank : ";
        }
        else{
            cout<<"Bank "<<i<<" : ";
        }
        cin>>input[i].name;
        indexOf[input[i].name] = i;
        int numTypes;
        cin>>numTypes;
        
        if(i==0) maxNumTypes = numTypes;
        
        string type;
        while(numTypes--){
            cin>>type;
            
            input[i].types.insert(type);
        }   
        
    }
    
    cout<<"Enter the number of transactions happening between the banks.\n";
    int numTransactions;
    cin>>numTransactions;
    
    vector<vector<int>> graph(number_of_banks,vector<int>(number_of_banks,0));
    
    cout<<"Enter the following details";
    cout<<"Debtor Bank , creditor Bank and amount\n";
    for(int i=0;i<numTransactions;i++){
        cout<<(i)<<" th transaction : ";
        string s1,s2;
        int amount;
        cin >> s1>>s2>>amount;
        
        graph[indexOf[s1]][indexOf[s2]] = amount;
    }
    minimizeCashFlow(number_of_banks,input,indexOf,numTransactions,graph,maxNumTypes);
    return 0; 
} 