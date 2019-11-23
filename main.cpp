#include "header.h"
#include <iostream>
#include <random>
#include <chrono>
#include <cstring>


using std::string, std::endl, std::cout, std::vector, std::stringstream;


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(12);

int randomnumber(int a, int b) {
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(generator);
}

class person {
public:
    string name = "";
    long moneyz;
    string publickey;
};

class transaction {
public:
    int sender;
    long amount;
    int reciever;


};

class block {
public:
    string Prev_Bock_Hash;
    string timestamp;
    string Version;
    string Merkel_Root;
    long Nonce;
    int difficulty_target;
    vector<transaction> Body_Transactions;
    block(string Prev_Bock_Hash, string Version, int difficulty_target,  vector<transaction> Body_Transactions)
    {
        Nonce = 0;
        this->Prev_Bock_Hash=Prev_Bock_Hash;
        this->Version=Version;
        this->difficulty_target=difficulty_target;
        this->Body_Transactions=Body_Transactions;
        this->timestamp="";
    }
};

vector<transaction> Choose_Transactions(vector<person> &Person, vector<transaction> &AllTransactions, int size) {
    vector<transaction> Chosen_Transactions;
    int transactionindex;
    vector<person> temp_person = Person;
    while ((!AllTransactions.empty()) && (Chosen_Transactions.size() < size)) {
        //cout<<temp_person[AllTransactions[transactionindex].sender].moneyz<<endl;

        transactionindex = randomnumber(0, AllTransactions.size()-1);

        if (temp_person[AllTransactions[transactionindex].sender].moneyz <
            AllTransactions[transactionindex].amount)
           AllTransactions.erase(AllTransactions.begin() + transactionindex);
        else {
            Chosen_Transactions.push_back(AllTransactions[transactionindex]);

            temp_person[AllTransactions[transactionindex].sender].moneyz -= AllTransactions[transactionindex].amount;
            AllTransactions.erase(AllTransactions.begin() + transactionindex);
        }
        //cout<<temp_person[AllTransactions[transactionindex].sender].moneyz<<endl;
    }

    Person = temp_person;

    return Chosen_Transactions;
}

std::string create_merkle(vector<transaction> transactions){
    std::vector<std::string> merkleTree;
    for (transaction t : transactions)
        merkleTree.push_back(inputhashing(std::to_string(t.amount)+std::to_string(t.sender)+std::to_string(t.reciever)));

    while (merkleTree.size() > 1){
        if (merkleTree.size()%2 != 0)
            merkleTree.emplace_back("");
        std::vector<std::string> newMerkleTree;
        for (int i = 0; i < merkleTree.size(); i += 2){
            std::string first = merkleTree.at(i);
            std::string second = merkleTree.at(i+1);
            newMerkleTree.push_back(inputhashing(first + second));
        }
        merkleTree = newMerkleTree;
    }
    return merkleTree.at(0);
}

string hash_block(block &Block) {
    string cstr;
    for (int i = 0; i < Block.difficulty_target; i++)
    {
        cstr.push_back('0');
    }
    int diff;
    string Hash;

    if(Block.timestamp.empty())
    {

        Block.Merkel_Root=create_merkle(Block.Body_Transactions);
        string a;

        do {

            Block.Nonce++;
            stringstream ss, date;
            date<<time(NULL);
            Block.timestamp=date.str();

            ss <<Block.Prev_Bock_Hash<<Block.difficulty_target<<Block.Version<< Block.timestamp <<Block.Merkel_Root<<Block.Nonce;

            Hash = inputhashing(ss.str());
            a=ss.str();
            diff = ((cstr.substr(0, Block.difficulty_target)).compare(Hash.substr(0, Block.difficulty_target)));
           // cout<<Hash.substr(0, Block.difficulty_target)<<endl;

        } while (diff!=0);

    }
    else
    {
        stringstream ss;
        ss <<Block.Prev_Bock_Hash<<Block.difficulty_target<<Block.Version<< Block.timestamp <<Block.Merkel_Root<<Block.Nonce;


        Hash = inputhashing(ss.str());


    }

    return Hash;

}



vector<person> Generate_People() {
    vector<person> Person;
    for (int i = 0; i < 1000; i++) {
        Person.push_back(person());
        int namelengh = randomnumber(4, 9);
        for (int j = 0; j < namelengh; j++) {
            int asciiletter = randomnumber(97, 122);

            Person[i].name += char(asciiletter);
        }
        Person[i].moneyz = randomnumber(100, 1000000);
    }
    return Person;
}

vector<transaction> Generate_Transactions() {
    vector<transaction> Transactions;
    for (int i = 0; i < 10000; i++) {
        Transactions.push_back(transaction());
        int senderid = randomnumber(0, 1000);
        int recieverid = randomnumber(0, 1000);
        while (senderid == recieverid) {
            recieverid = randomnumber(0, 1000);
        }
        int amount = randomnumber(100, 1000000);
        Transactions[i].sender = senderid;
        Transactions[i].reciever = recieverid;
        Transactions[i].amount = amount;
    }
    return Transactions;
}


 void add_block_to_blockchain( vector<block> &Blockchain ,block Block, vector<person> &Person)
 {
    Blockchain.push_back(Block);

    for(auto Tra: Block.Body_Transactions)
    {
        Person[Tra.reciever].moneyz+=Tra.amount;
    }

}


int main() {

    int size = 100;
    int difficulty_target = 2;
    string version = "0.2";
    vector<block> Blockchain;
    vector<person> Person = Generate_People();
    vector<transaction> Transactions = Generate_Transactions();


    vector<transaction> Transactions_ready_for_block = Choose_Transactions(Person, Transactions, size);
    block Genesis = *new block("00000000000000000000000000000000",version,difficulty_target,Transactions_ready_for_block);
    string block_hash=hash_block(Genesis);

    add_block_to_blockchain(Blockchain,Genesis, Person);
    while(!Transactions.empty())
    {
       Transactions_ready_for_block = Choose_Transactions(Person, Transactions, size);
       block new_block= *new block(hash_block(Blockchain.back()),version, difficulty_target,Transactions_ready_for_block);
       string block_hash=hash_block(new_block);
       add_block_to_blockchain(Blockchain,new_block, Person);
       cout<<block_hash<<endl;

    }
        return 0;
}