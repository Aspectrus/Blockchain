#include "header.h"
#include <iostream>
#include <random>
#include <chrono>
#include <cstring>


using std::string, std::endl, std::cout, std::vector, std::stringstream;


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(541541);

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
    string timestamp;
    string hash;

};

class block {
public:
    string Current_Hash;
    string Prev_Bock_Hash;
    string timestamp;
    string Version;
    string Merkel_Root;
    long Nonce;
    int difficulty_target;
    vector<transaction> Body_Transactions;
    block(string Prev_Bock_Hash, string Version, int difficulty_target,  vector<transaction> Body_Transactions)
    {
        this->Nonce = 0;
        this->Prev_Bock_Hash=Prev_Bock_Hash;
        this->Version=Version;
        this->difficulty_target=difficulty_target;
        this->Body_Transactions=Body_Transactions;
        this->timestamp="";
        this->Current_Hash="";
    }
};

vector<transaction> Choose_Transactions(vector<person> Person, vector<transaction> &AllTransactions, int size) {
    vector<transaction> Chosen_Transactions;
    int transactionindex;
    vector<person> temp_person = Person;
    while ((!AllTransactions.empty()) && (Chosen_Transactions.size() < size)) {
        transactionindex = randomnumber(0, AllTransactions.size()-1);
    int diff =AllTransactions[transactionindex].hash.compare(inputhashing((Person[AllTransactions[transactionindex].reciever].publickey)+
    std::to_string(AllTransactions[transactionindex].sender)+
    std::to_string(AllTransactions[transactionindex].reciever)+
    std::to_string(AllTransactions[transactionindex].amount)+
    (AllTransactions[transactionindex].timestamp)));



        if ((temp_person[AllTransactions[transactionindex].sender].moneyz <AllTransactions[transactionindex].amount)||(diff!=0))
            AllTransactions.erase(AllTransactions.begin() + transactionindex);
        else {
            Chosen_Transactions.push_back(AllTransactions[transactionindex]);

            temp_person[AllTransactions[transactionindex].sender].moneyz -= AllTransactions[transactionindex].amount;
            temp_person[AllTransactions[transactionindex].sender].moneyz += AllTransactions[transactionindex].amount;

        }

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

string hash_block(block &Block, long mining_length) {


    string cstr;
    for (int i = 0; i < Block.difficulty_target; i++)
    {
        cstr.push_back('0');
    }
    int diff=-1;
    string Hash;

    if(Block.Current_Hash.empty())
    {
        Block.Merkel_Root=create_merkle(Block.Body_Transactions);
        string a;
        for(;Block.Nonce<100000*mining_length; Block.Nonce++)
         {

            stringstream ss, date;
            date<<time(NULL);
            Block.timestamp=date.str();
            ss <<Block.Prev_Bock_Hash<<Block.difficulty_target<<Block.Version<< Block.timestamp <<Block.Merkel_Root<<Block.Nonce;
            Hash = inputhashing(ss.str());

            diff = ((cstr.substr(0, Block.difficulty_target)).compare(Hash.substr(0, Block.difficulty_target)));

            if(diff==0)  {
                Block.Current_Hash=Hash;
            return Hash;}
        }

        Block.Current_Hash="";
        return "";
    }
    else
    {
        stringstream ss;
        ss <<Block.Prev_Bock_Hash<<Block.difficulty_target<<Block.Version<< Block.timestamp <<Block.Merkel_Root<<Block.Nonce;
        Hash = inputhashing(ss.str());

        return Hash;
    }
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
        Person[i].publickey = inputhashing(std::to_string(i)+ Person[i].name);

    }
    return Person;
}

vector<transaction> Generate_Transactions(vector<person> Person) {
    vector<transaction> Transactions;

    for (int i = 0; i < 10000; i++) {
        stringstream date;
        date<<time(NULL);

        Transactions.push_back(transaction());
        int senderid = randomnumber(0, 999);
        int recieverid = randomnumber(0, 999);
        while (senderid == recieverid) {
            recieverid = randomnumber(0, 999);
        }
        int amount = randomnumber(100, 1000000);

        Transactions[i].sender = senderid;
        Transactions[i].reciever = recieverid;
        Transactions[i].amount = amount;
        Transactions[i].timestamp=date.str();
        Transactions[i].hash = inputhashing(Person[recieverid].publickey+
                std::to_string(Transactions[i].sender)+
                std::to_string(Transactions[i].reciever)+
                std::to_string(Transactions[i].amount)+
                (Transactions[i].timestamp));
    }
    return Transactions;
}


void add_block_to_blockchain(vector<block> &Blockchain, block Block, vector<person> &Person,vector<transaction> &Transactions)
 {
    Blockchain.push_back(Block);
    for( auto Tra: Block.Body_Transactions)
    {
        Person[Tra.reciever].moneyz+=Tra.amount;
        Person[Tra.sender].moneyz-=Tra.amount;

        for( int i=0; i<Transactions.size(); i++)
        {
            if(Tra.hash==Transactions[i].hash) Transactions.erase(Transactions.begin()+i);
        }

    }

}

block block_mining(vector<block> Blockchain, vector<transaction> &Transactions, vector<person> Person, int difficulty_target)
{

    string version = "0.2";

    block BlockA = *new block(Blockchain.back().Current_Hash,version,difficulty_target,Choose_Transactions(Person, Transactions, 100));
    block BlockB = *new block(Blockchain.back().Current_Hash,version,difficulty_target,Choose_Transactions(Person, Transactions, 100));
    block BlockC = *new block(Blockchain.back().Current_Hash,version,difficulty_target,Choose_Transactions(Person, Transactions, 100));
    block BlockD = *new block(Blockchain.back().Current_Hash,version,difficulty_target,Choose_Transactions(Person, Transactions, 100));
    block BlockE = *new block(Blockchain.back().Current_Hash,version,difficulty_target,Choose_Transactions(Person, Transactions, 100));


    string block_hash ="";
    long mining_length = 1;
    while(1)
    {
        block_hash=hash_block(BlockA,mining_length);
        if(!BlockA.Current_Hash.empty())
        {

            return BlockA;
        }

        block_hash=hash_block(BlockB,mining_length);
        if(!BlockB.Current_Hash.empty()) {

            return BlockB;

        }
        block_hash=hash_block(BlockC,mining_length);
        if(!BlockC.Current_Hash.empty()) {

            return BlockC;

        }
        block_hash=hash_block(BlockD,mining_length);
        if(!BlockD.Current_Hash.empty()) {

            return BlockD;

        }
        block_hash=hash_block(BlockE,mining_length);
        if(!BlockE.Current_Hash.empty()) {

            return BlockE;
        }
        mining_length++;
    }



}



int main() {
    int mining_length=100, choice;
    int size = 100;
    int difficulty_target = 2;
    string version = "0.2";
    vector<block> Blockchain;
    menu();
    std::cin >> choice;
    cout << "Choice is " << choice << endl;

    // Genesis Block
    vector<person> Person = Generate_People();
    vector<transaction> Transactions = Generate_Transactions(Person);
    vector<transaction> Transactions_ready_for_block = Choose_Transactions(Person, Transactions, size);
    block Genesis = *new block("00000000000000000000000000000000",version,difficulty_target,Transactions_ready_for_block);
    string block_hash=hash_block(Genesis, mining_length);
    add_block_to_blockchain(Blockchain, Genesis, Person, Transactions);


    switch (choice) {
        //v0.1
        case 1: {

            cout<<"difficulty_target?"<<endl;
            cin>>difficulty_target;
            while(!Transactions.empty())
            {
                Transactions_ready_for_block = Choose_Transactions(Person, Transactions, size);
                block new_block= *new block(hash_block(Blockchain.back(),mining_length),version, difficulty_target,Transactions_ready_for_block);
                string block_hash=hash_block(new_block, mining_length);
                add_block_to_blockchain(Blockchain, new_block, Person, Transactions);
                cout<<block_hash<<endl;
            }

            break;
        }
            //v0.2+v0.3
        case 2: {
            cout<<"difficulty_target?"<<endl;
            cin>>difficulty_target;
            while(!Transactions.empty()) {

                block new_block = block_mining(Blockchain, Transactions, Person, difficulty_target);


                add_block_to_blockchain(Blockchain, new_block, Person, Transactions);
                cout << new_block.Current_Hash << endl;

            }

            break;
        }

        default:
            printf("Wrong Input\n");
    }


   return 0;
}