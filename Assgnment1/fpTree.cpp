#include<iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <queue>
#include <map>
#include<cmath>
#include<list>



using namespace std;
using namespace std::chrono;






// frequency of each item in database, this variable will be filled in 1st databse scan
unordered_map<int, int> freq;


/**
 * helper function: update frequency of each item given tansaction, during 1st database scan.
 * @param line [Single transaction string]
 * @param freq [frequency map to be updated for each item in line]
 */
void update_frequency(string & line, unordered_map<int, int> & freq)
{
	stringstream ss(line);

	string tok;

	while(getline(ss, tok, ' '))
	{
		freq[stoi(tok)]++;
	}

}






// helper function: convert string to vector of int
vector<int> split(string str, unordered_map<int, int> & freq, int min_support)
{
	vector<int> internal;
	stringstream ss(str);
	string tok;
	while(getline(ss, tok, ' ')){

    	// ignore if support is less than min_support else consider
		// if(freq.find(stoi(tok)) != freq.end() && freq[stoi(tok)] >= min_support){
		// 	internal.push_back(stoi(tok));
		// }

		if(freq.count(stoi(tok)) >= 1 && freq[stoi(tok)] >= min_support){
			internal.push_back(stoi(tok));
		}

	}
	return internal;
}



bool asciiCmp(int a, int b){
	return to_string(a) <= to_string(b);
}


void printFrequent(vector<int> & arr){
	for(auto el: arr){
		cout << el << " ";
	}
	cout << "\n";
}




/**
 * Class Node
 * Representation of sinlge node.
 * Node will have:
 * 	item
 * 	count of item
 * 	children: map of children pointer
 * 	parent: to traverse the tree upward
 */
class FpNode
{
public:
	int item;
	int count = 0;
	unordered_map<int, FpNode *> children;
	FpNode * parent;


	FpNode(int item, FpNode * parent){
		this->item = item;
		this->parent = parent;
	}

	FpNode* getParent(){
		return this->parent;
	}

	FpNode* next(int item){
		// return child node which has value item
		// if(children.find(item) != children.end()){
		// 	return this->children[item];
		// }

		if(children.count(item) >= 1){
			return this->children[item];
		}

		return nullptr;
	}

};



/**
 * Class FpTree
 * A Fptree have:
 * 		root node,
 * 		frequency of each item of this fp tree,
 * 		header table: for each items, it store list of node pointer.
 *
 *  We can insert a vector of items in FP tree
 */
class FpTree
{
public:
	FpNode* root;


	// item->support
	unordered_map<int, int> freq_items;

	// item->support,node list
	unordered_map<int, vector<FpNode*>> header_table;

	// bool isSinglePath = true;



	FpTree(){
		root = new FpNode(-1, nullptr);
	}

	void insert(vector<int> tx){
		// insert transaction(vector) into tree

		if(not tx.size()) return;

		FpNode* current = this->root->next(tx[0]);

		if(current == nullptr){
			this->root->children[tx[0]] = new FpNode(tx[0], this->root);
		}

		// this->root->children.size() == 1
		// if(this->root->children.size() >= 2) isSinglePath = false;

		current = this->root->next(tx[0]);
		current->count += 1;
		this->freq_items[tx[0]]++;

		this->header_table[tx[0]].push_back(current);


		for(int i = 1; i < tx.size(); i++){
			int item = tx[i];
			if(current->next(item) == nullptr){
				current->children[item] = new FpNode(item, current);
			}

			// this->current->children.size() == 1
			// if(current->children.size() >= 2) isSinglePath = false;

			current = current->next(item);
			current->count += 1;
			this->freq_items[tx[i]]++;
			this->header_table[tx[i]].push_back(current);
		}
	}
};



// This variable store our final answer of all frequent items
// TODO (store in variable and write to file in end) or directly write to file
vector<vector<int>> allFrequent;


/**
 * Generate all 2^n combination path vector. n = length of path
 * append alpha suffix to each 2^n generated combination
 * Finally sort each combination in alphabatical order and append to our final anwer allFrequent
 *
 * @param path [vector of item for which we have to genrate all combination]
 * @param alpha [suffix to be appended to all generated combination]
 */



void backTrack(vector<int> &path, vector<int> &curr, vector<int> &alpha, int index)
{
    // cout<<" index is "<<index<<endl;
    if(index==path.size())
    {
        vector<int> comb;
        comb.insert(comb.end(),curr.begin(), curr.end());
        comb.insert(comb.end(), alpha.begin(), alpha.end());
        sort(comb.begin(), comb.end(), asciiCmp);

        // allFrequent.push_back(comb);
        printFrequent(comb);
        return ;
    }
    curr.push_back(path[index]);
    backTrack(path, curr, alpha, index+1);
    curr.pop_back();
    backTrack(path, curr, alpha, index+1);
}

void generateCombinations(vector<int> &path, vector<int> &alpha)
{
    int sz = path.size();
    // cout<<" sz is "<<sz<<endl;
    for(int i = 0 ; i < sz ; i++)
    {
      // cout<<" i here is "<<i<<endl;
      vector<int> curr;
      curr.push_back(path[i]);
      backTrack(path, curr, alpha, i+1);
      curr.clear();
    }
}

// void generateCombinations(vector<int> path, vector<int> alpha)
// {
// 	int n = path.size();
// 	// cout<<" isme jb aaya n ki value hai "<<n<<endl;
// 	// cout<<" print to kru maxm value "<<(1<<n)<<endl;
//   // cout<<(1<<33)<<endl;
// 	int l = 1;
// 	for(int mask = 1; mask < (1 << n); ++mask)
// 	{
// 		int curr_mask = mask;
// 		vector<int> comb;
// 		for(int i = 0; i < n; i++)
// 		{
// 			if(curr_mask % 2 == 1)
// 			{
// 				comb.push_back(path[i]);
// 			}
// 			curr_mask = (curr_mask >> 1);
// 			if(curr_mask == 0) break;
// 		}
// 		comb.insert(comb.end(), alpha.begin(), alpha.end());

// 		sort(comb.begin(), comb.end(), asciiCmp);

// 		allFrequent.push_back(comb);
// 		if((mask % 10000) == 0)
// 		{
// 			cout<<l<<endl;
// 			l++;
// 		}
// 	}
// }



/**
 * Generate conditional FP-tree for given item using given FP-tree.
 *
 * @param fpCondTree_orig Given orignal FP-tree
 * @param item_name [Generate condtional FP-tree for this item]
 * @param freq_item [frequency of items in given original FP-Tree]
 * @param header_table [Header table associated with original FP-Tree]
 * @param min_support [required min support]
 * @return [return condotional FP-tree]
 */
FpTree * constructCondFpTree(FpTree * & fpCondTree_orig, int item_name, unordered_map<int, vector<FpNode*>> & header_table, unordered_map<int, int>  & freq_item, int min_support){
	FpTree * new_cond_fptree = new FpTree();

	//visit header table and insert branch

	// vector<vector<int>> paths; 
	vector<list<int>> paths; 

	// freq each item in new conditional fp tree
	unordered_map<int, int> freq_cond;
	for(auto node_ptr: header_table[item_name]){
		// visit item_ptr and move upto root, store path and build tree
		FpNode * curr_node = node_ptr->getParent();

		list<int> path;
		

		while(curr_node->getParent()){
			if(freq_item[curr_node->item] >= min_support){
				freq_cond[curr_node->item]++;
				path.push_back(curr_node->item);
			}
			curr_node = curr_node->getParent();
		}
		reverse(path.begin(), path.end());


		// we don't need to sort, bcoz after reverse path will be in decresing order of support
		// sort(path.begin)

		if(path.size() > 0){
			paths.push_back(path);
		}
	}


	for(auto lst: paths){

		// remove item from list if it is less than min support
		lst.remove_if([&freq_cond, min_support](int el){
			return freq_cond[el] < min_support;
		});
		vector<int> new_path(lst.begin(), lst.end());
		if(new_path.size() > 0){
			new_cond_fptree->insert(new_path);
		}
	}

	return new_cond_fptree;
}




/**
 * Convert unordered_map<int, int> (item_name->frequency) to vector<pair<int, int>> (item_name, frequency). vector will be in sorted order decreasing frequency.
 *
 * @param freq_item [unordered_map of item_name to frequency]
 * @return [return vector<pair<int, int>>, Pair is (item_name, frequency)]
 */
vector<pair<int, int>> sortByFrequency(unordered_map<int, int> & freq_item){

	vector<pair<int, int>> elems(freq_item.begin(), freq_item.end());
	sort(elems.begin(), elems.end(), [](pair<int, int> e1, pair<int, int> e2){
		return e1.second == e2.second ? e1.first < e2.first : e1.second > e2.second;
	});

	return elems;
}



/**
 * Check if given FP-tree contains only one path
 *
 * @param ftree [Given FP-Tree]
 * @return [true if it contains only one path]
 */
bool isSinglePath(FpTree * & ftree){
	// return ftree->isSinglePath;
	// if every node has 1 or less child
	unordered_map<int, vector<FpNode*>> header_table = ftree->header_table;

	FpNode * curr = ftree->root;
	while(true){
		unordered_map<int, FpNode *> children = curr->children;
		if(children.size() > 1) return false;

		if(children.size() == 0) return true;

		curr = children.begin()->second;
	}

	return false;

	return true;

}



/**
 * Recursivly generate all frequent pattern with given suffix alpha and minimum support
 *
 * @param fpCondTree [Given conditional FP-Tree]
 * @param alpha [Suffix, we have already considered in given conditional FP-Tree]
 * @param min_support [min required support]
 */
void fpGrowth(FpTree * & fpCondTree, vector<int> alpha, int min_support)
{
	// cout<<" aa rha hai kya "<<endl;
	// cout<<" alpha ka size hai "<<alpha.size()<<endl;
	// cout<<"  min support is "<<min_support<<endl;

	/**
		 (1) if Tree contains a single path P then
		     (1.1) for each combination (denoted as β) of the nodes in the path P
		     (1.2) generate pattern β ∪ α with support count = minimum support count of nodes in β;
	 */

	// cout << "min_support";
	unordered_map<int, vector<FpNode*>> header_table = fpCondTree->header_table;
	unordered_map<int, int> freq_items = fpCondTree->freq_items;
	vector<pair<int, int>> sorted_freq_items = sortByFrequency(freq_items);

	// cout << freq_items.size() << " ";

	if(isSinglePath(fpCondTree))
	{
		// cout<<" aayaa isme "<<endl;
		vector<int> path;

		FpNode * curr_node = fpCondTree->root;
		if(curr_node->children.size() == 0) return;
		curr_node = curr_node->children.begin()->second;

		// int temp = 0;
		while(curr_node)
		{
			// cout<<" temp is "<<temp;
			// temp++;
			if(freq_items[curr_node->item] >= min_support)
				path.push_back(curr_node->item);
			else
				break;
			unordered_map<int, FpNode *> children = curr_node->children;
			if(children.size() == 0) break;
			curr_node = children.begin()->second;
		}

		// cout<<" bahar aao "<<endl;
		// Now we have got path. Generate all combinatio ms
		generateCombinations(path, alpha);

	}

	/**
	  	(2)else for each ai in the header of Tree {
		 	(2.1) generate pattern β = ai ∪ α with support count = ai .support count;
		 	(2.2) construct β’s conditional pattern base and then β’s conditional FP tree Treeβ ;
		 	(2.3) if Treeβ != ∅ then
		 	(2.4) call FP growth(Treeβ , β); }

	 */


	else
	{
		// cout<<" size is "<<sorted_freq_items.size()<<endl;
		for(int i = sorted_freq_items.size() - 1; i >= 0 ; i--){

			int item = sorted_freq_items[i].first;
			int support = sorted_freq_items[i].second;

			if(support >= min_support){
    			// generate pattern β = ai ∪ α

				vector<int> pattern(alpha.begin(), alpha.end());
				if(find(alpha.begin(), alpha.end(), item) == alpha.end()) pattern.push_back(item);

				sort(pattern.begin(), pattern.end(), asciiCmp);
				// allFrequent.push_back(pattern);
				printFrequent(pattern);

    			// construct β’s conditional pattern base and then β’s conditional FP tree Treeβ
    			// Q: How to construct β’s cond FpTree?
    			// A: build by considering leaf as 'item'

				FpTree * tree_beta = constructCondFpTree(fpCondTree, item, header_table, freq_items, min_support);


    			// 	if Treeβ != ∅ then
				// 		call FP growth(Treeβ , β);

				if(tree_beta->header_table.size() == 0) continue;

				fpGrowth(tree_beta, pattern, min_support);

			}
		}


	}
}



// void vvprint(vector<vector<int>> arr){
// 	cout << "\n-----------\n";
// 	for(auto v: arr){
// 		for(auto e: v){
// 			cout << e << " ";
// 		}
// 		cout << "\n";
// 	}
// }





// void writeFile(string out_file){
// 	// cout << "\nTotal frequent items = " << allFrequent.size() << "\n";
// 	// cout << "writing to file";

// 	ofstream output_file(out_file);

// 	for(auto itemSet: allFrequent){
// 		for(auto item: itemSet){
// 			output_file << item << " ";
// 		}
// 		output_file << "\n";
// 	}
// }


int main(int argc, char* argv[])
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	auto start = high_resolution_clock::now();

	// name, percent, filane
	// if(argc < 3){
	// 	std::cerr << "Usage: " << argv[0] << " X" << " FILE_NAME" << " OUT_FILE" << endl;
	// 	return 1;
	// }

	string file_name = argv[1];
	float support_percent = stof(argv[2]);
	
	string out_file = argv[3];

	// cout << "file_name = " << file_name << "\n";
	// cout<<"support_percent % = "<<support_percent<<endl;
	// cout<<"out file % = "<<out_file<<endl;


	

	// file name given, (to read from command line)
	// string file_name = "data501";
	// string file_name = "webdocs.dat";
	// string file_name = "retail.dat";
	

	ifstream infile(file_name.c_str());


	// x % support threshold given, (to be read from command line)
	// float support_percent = 20;

	// First database scan: count frequency of each item
	int total_tansaction = 0;

	if(infile.is_open())
	{
		string line;
		while(getline(infile, line))
		{
			update_frequency(line, freq);
			total_tansaction++;
		}
		infile.close();

	}

	int min_support = ceil(((float)support_percent / 100) * total_tansaction);

	// cout << "Min support = " << min_support << "\n";
	// cout << "unique items = " << freq.size() << "\n";






	// make fp tree in order of support


	// 2nd database scan: read file line by line and make fptree

	ifstream infile2(file_name.c_str());

	FpTree* ftree = new FpTree();

	if(infile2.is_open()){

		for(string line; getline(infile2, line);){
			if(line.empty()) break;
			vector<int> tx = split(line, freq, min_support);

			sort(tx.begin(), tx.end(), [](int item1, int item2){
				return freq[item1] == freq[item2] ? item1 < item2 : freq[item1] > freq[item2];
			});
			ftree->insert(tx);
		}

		infile2.close();
	}


	// if(infile2.is_open()) infile2.close();

	// cout << "size of header is " << ftree->header_table.size() << "\n";

	// cout << "fp growth\n";

	fpGrowth(ftree, {}, min_support);


	// print in terminal
	// vvprint(allFrequent);

	// Write into file
	// writeFile(out_file);



	// print total time taken
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	// cout << "\nTime: " << duration.count() << " s" << endl;

	return 0;
}
