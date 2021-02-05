#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<utility>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<stack>
#include<cmath>
#include<cstring>
#include<chrono>
// #include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

// important function for using vector with the set data structures
struct VectHashing {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i =0;i<v.size();i++) {
            seed ^= hasher(v[i]) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};
///////////////////////////////////////////////////////

// getting vector of integer from given string
vector<int> getItemval(string rdata)
{
	vector<int> sval;

	int i,j,k;
	int n = rdata.size();

	string data = "";

	for (i=0;i<n;i++)
	{
		if (rdata[i] == ' ')
		{
			if (data != "")
			{
				k = stoi(data);
				sval.push_back(k);
				data = "";
			}
		}
		else
			data.push_back(rdata[i]);
	}

	if (data != "")
	{
		k = stoi(data);
		sval.push_back(k);
		data = "";
	}

	return sval;
}

// get intersection length of two vector 
// used for doing join of two vector during candiate generation step
int getIntersectlen(vector<int>& v1, vector<int>& v2)
{
	int i;
	int count = 0;

	for (i=0;i<v1.size();i++)
	{
		if (v1[i]==v2[i])
			count++;
		else 
			return count;
	}

	return count;
}

// get Count for each itemset in the vintmed vector 
// getting frequcey of each itemset in the vintmed vector 
vector<int> getCountOfEachItemSet(string filename, vector<vector<int>>& vintmed, unordered_map<int,int>& mp3, int diffval2)
{
	fstream rdfile;
	rdfile.open(filename,ios::in);

	int n = vintmed.size();
	vector<int> vcount;
	vcount.resize(n, 0);

	int veclen = vintmed.size();
	int lastRowlen = vintmed[veclen-1].size();
	int lastRowElem = vintmed[veclen-1][lastRowlen-1];


	// cout<<" size of vintmed = "<<vintmed.size()<<endl;

	// cout<<" printing the vintmed vector :- "<<endl;
	// for (int i=0;i<vintmed.size();i++)
	// {
	// 	for (int j=0;j<vintmed[i].size();j++)
	// 		cout<<vintmed[i][j]<<" ";
	// 	cout<<endl;
	// }

	if ( rdfile.is_open())
	{
		string rdata;
		vector<int> vtemp;
		vector<int> vchecker;

		int minimum = 2147483647;
		int maximum = -2147483648;

		int i,j,k,u,v;

		while( getline(rdfile, rdata))
		{
			vtemp = getItemval(rdata);
			vchecker.resize(diffval2,0);

			// sort(vtemp.begin(),vtemp.end());

			int elmcnt = 0;
			int minval = minimum;
			int maxval = maximum;

			for (i=0;i<vtemp.size();i++)
			{
				k = vtemp[i];

				if (mp3.find(k) != mp3.end())
				{
					u = mp3[k];
					vchecker[u] = 1;
					elmcnt++;

					if (minval > k)
						minval = k;

					if (maxval < k)
						maxval = k;
				}
			}

			if (lastRowElem >= minval)
			{
				int mncond = 0;

				for (i=0;i<vintmed.size();i++)
				{

					int vinlen = vintmed[i].size();

					int firstval = vintmed[i][0];
					int lastval = vintmed[i][vinlen-1];

					if (elmcnt >= vinlen)
					{
						if ( minval <= firstval && lastval <= maxval )
						{
							int chcond = 0;

							for (j=0;j<vinlen;j++)
							{
								k = vintmed[i][j];
								u = mp3[k];

								if (vchecker[u]==0)
								{
									chcond = 1;
									break;
								}
							}

							if (chcond == 0)
								vcount[i]++;
					    }
					    else if (maxval < firstval)
					    {
					    	mncond = 1;
					    }
				    }

				    if (mncond==1)
				    	break;
				}

		    }

			vchecker.clear();
		}
	}

	rdfile.close();

	return vcount;
}

// main function 
int main(int argc, char *argv[])
{
	auto start = high_resolution_clock::now();

	// cout<<" Apriori algorithm "<<endl;
	// Code is made for very small test case 
	// Here threshold parameter of percentage is not coded. i have taken threshold value of 2 given in slides
	// parameter are just initilized , but they should be given by terminal, so we have to fix these things.
	// If there is a chance for optimization, tell me.
	// Understand the use of various maps and set considered in this program.

	if (argc < 4)
	{
		// ./RollNo.sh retail.dat X  <filename>” - correct input format
		// cout<<" input format is incorrect "<<endl;
		return 1;
	}

	// initialize variable
	string filename = "";
	float thresPcent = 0.0;
	string outfilename = "";

	// file for reading data
	fstream rfile1;  
	filename = argv[1];  // file name of input file
	thresPcent = stof(argv[2]);   //  threshold value
	outfilename = argv[3];

	rfile1.open(filename,ios::in);

	int count = 0;  // total no. of transaction
	int diffval = 0; // total differnet itemids
	unordered_map<int,int> mp1,mp2;
	int i,j,k,u,v;
	vector<int> numcnt; // storing count of all differnet items

	// mp1 map for -  (item id -- index value)
	// mp2 map for  - (index value -- item id)
	// counting no of transaction 
	// counting no of items involved in all transactions
	if (rfile1.is_open())
	{
		string rdata;
		vector<int> vtemp;
		
		while( getline(rfile1,rdata))
		{
			vtemp = getItemval(rdata);
			// cout<<" for line - "<<count<<endl;

			for (i=0;i<vtemp.size();i++)
			{
				k = vtemp[i];
				// cout<<k<<" ";

				if (mp1.find(k) == mp1.end())
				{
					mp1[k] = diffval;
					mp2[diffval] = k;
					diffval++;
					numcnt.push_back(1);
				}
				else
				{
					u = mp1[k];
					numcnt[u]++;
				}
			}
			// cout<<endl;

			vtemp.clear();

			count++;
		}
	}
	rfile1.close();

	///////////////////////////////////////////////////// printing intermediate result 
	////////////////////////////////////////////////////////

	float thresval = thresPcent;
	// cout<<" thresval = "<<thresval<<endl;
	// cout<<" total line = "<<count<<endl;
	// cout<<" diffval = "<<diffval<<endl;

	unordered_map<int,int>::iterator itr;

	// cout<<" printing the first map values :-"<<endl;
	// for (itr = mp1.begin(); itr != mp1.end(); itr++)
	// 	cout<<itr->first<<" "<<itr->second<<endl;
	// cout<<endl;

	// cout<<" printing the second map values :-"<<endl;
	// for (itr = mp2.begin(); itr != mp2.end(); itr++)
	// 	cout<<itr->first<<" "<<itr->second<<endl;
	// cout<<endl;

	int mxvalue = 0;
	int mxvalind = -1;
	// cout<<" printing the count of different value :-"<<endl;
	for (i=0;i<numcnt.size();i++)
	{
		// cout<<mp2[i]<<" "<<numcnt[i]<<endl;

		if (mxvalue < numcnt[i])
		{
			mxvalue = numcnt[i];
			mxvalind = mp2[i];
		}
	}
	// cout<<endl;

	// cout<<" mxvalind = "<<mxvalind<<endl;
	// cout<<" mxvalue = "<<mxvalue<<endl;

	///////////////////////////////////////////////////// printing intermediate result 
	////////////////////////////////////////////////////////

	
	int diffval2 = 0;  // storing count of differnet item its whose frequncy satisfies threshold condtion.
	// mp3 map for -  (item id -- index value)
	// mp4 map for  - (index value -- item id)
	unordered_map<int,int> mp3,mp4;
	

	vector<vector<int>> vprev, vintmed, vcur, vtotal;
	vector<int> vtotLen; // storing frequecy in itemset in final solution
	// vprev - used for storing actual frequecy set (fk-1) 
	// vintmed - storing candiate set (ck)
	// vcur - storing frequency set (fk)
	// vtotal - storing all the frequcency set (from size 2 to k)

	float itemPcent; // storing itemset percentage with respect to total no. of transaction

	for (i=0;i<numcnt.size();i++)
	{
		itemPcent = (float(numcnt[i] * 100))/((float)(count));

		if (itemPcent >= thresval)
		{
			// cout<<" itemPcent = "<<itemPcent<<endl;
			k = mp2[i];
			// cout<<" k = "<<k<<endl;
			mp3[k] = diffval2;
			mp4[diffval2] = k;
			vprev.push_back({k});
			diffval2++;
		}
	}



	///////////////////////////////////////////////////// printing intermediate result 
	////////////////////////////////////////////////////////
	// cout<<" no. of initial candiate elements :- "<<diffval2<<endl;

	// cout<<" printing the third map values :-"<<endl;
	// for (itr = mp3.begin(); itr != mp3.end(); itr++)
	// 	cout<<itr->first<<" "<<itr->second<<endl;
	// cout<<endl;

	// cout<<" printing the fourth map values :-"<<endl;
	// for (itr = mp4.begin(); itr != mp4.end(); itr++)
	// 	cout<<itr->first<<" "<<itr->second<<endl;
	// cout<<endl;

	// cout<<" printing initial candiate for frequent itemset :- "<<endl;
	// for (i=0;i<vprev.size();i++)
	// {
	// 	for (j=0;j<vprev[i].size();j++)
	// 		cout<<vprev[i][j]<<" ";
	// 	cout<<endl;
	// }
	// cout<<endl;

	///////////////////////////////////////////////////// printing intermediate result 
	////////////////////////////////////////////////////////

	vector<int> vtkp1, vtkp2, vnew, vtemp; // temperory storage vectors 
	unordered_set <vector<int>, VectHashing> st; // used for pruning step of candiate generation

	int vlen; // for the size of element of vprev elment
	int vinslen; // for knowing the intersecting length of two vectors
	int mcond; // used for checking subset of newly formed vector
	int ctr; // used for storing length of newly formed vector

	vector<int> vlenItSet; // storing frequecy of each itemset in candiate set (or vintmed vector )
	

	sort(vprev.begin(),vprev.end());

	for (i=0;i<vprev.size();i++)
	{
		k = vprev[i][0];
		vtotal.push_back({k});
		u = mp1[k];
		vtotLen.push_back(numcnt[u]);
	}

	// cout<<" vtotal.size() = "<<vtotal.size()<<endl;

	// forming all the frequent itemset 
	while (vprev.size()!=0)
	{
		vlen = vprev[0].size();

		if (vlen==1)  // for frequecnt set of size 1
		{
			// making of candiate set 
			for (i=0;i<vprev.size();i++)
			{
				for (j=i+1;j<vprev.size();j++)
				{
					vtkp1 = vprev[i];
					vtkp2 = vprev[j];

					vnew.push_back(vtkp1[0]);
					vnew.push_back(vtkp2[0]);
					vintmed.push_back(vnew);
					vnew.clear();
				}
			}

			// filtering step , checking whether element vector of candiate set satisfies the frequecy condition or not
			if (vintmed.size()!=0)
			{
				// cout<<" size of vintmed = "<<vintmed.size()<<endl;

				vlenItSet = getCountOfEachItemSet( filename, vintmed, mp3, diffval2);

				for (i=0;i<vlenItSet.size();i++)
				{
					itemPcent = (float(vlenItSet[i] * 100))/((float)(count));

					if (itemPcent >= thresval)
					{
						vtemp = vintmed[i];
						vcur.push_back(vtemp);
						vtotal.push_back(vtemp);
						st.insert(vtemp);
						vtotLen.push_back(vlenItSet[i]);
					}
				}

				vintmed.clear();
			}
		}
		else  // for frequecnt set of size more than 1
		{
			// making of candiate set 
			for (i=0;i<vprev.size();i++)
			{
				for (j=i+1;j<vprev.size();j++)
				{
					vtkp1 = vprev[i];
					vtkp2 = vprev[j];

					vinslen = getIntersectlen( vtkp1, vtkp2);

					if (vinslen == vlen-1)
					{
						vnew = vtkp1;
						vnew.push_back(vtkp2[vlen-1]);

						mcond = 0;

						ctr = vlen+1;

						// pruning step , checking whether (k-1) subset of formed candiate element vector exist in the set or not
						while (ctr > 0)
						{
							if (ctr == (vlen+1))
							{
								if (st.find(vtkp1) == st.end())
									mcond = 1;
							}
							else
							{
								vtkp1[ctr-1] = vnew[ctr];

								if (st.find(vtkp1) == st.end())
									mcond = 1;
							}

							if (mcond==1)
								break;

							ctr--;
						}

						if (mcond==0)
						{
							vintmed.push_back(vnew);
						}

						vnew.clear();
					}
					else
					{
						break;
					}
				}
			}

			// filtering step , checking whether element vector of candiate set satisfies the frequecy condition or not
			if (vintmed.size() != 0)
			{
				// cout<<" size of vintmed = "<<vintmed.size()<<endl;
				
				vlenItSet = getCountOfEachItemSet(filename, vintmed, mp3, diffval2);

				for (i=0;i<vlenItSet.size();i++)
				{
					itemPcent = (float(vlenItSet[i] * 100))/((float)(count));

					if (itemPcent >= thresval)
					{
						vtemp = vintmed[i];
						vcur.push_back(vtemp);
						vtotal.push_back(vtemp);
						st.insert(vtemp);
						vtotLen.push_back(vlenItSet[i]);
					}
				}

				vintmed.clear();
			}
		}

		vprev.clear();
		vprev = vcur;
		vcur.clear();

		mp3.clear();
		diffval2 = 0;

		for (i=0;i<vprev.size();i++)
		{
			for (j=0;j<vprev[i].size();j++)
			{
				k = vprev[i][j];

				if (mp3.find(k)==mp3.end())
				{
					mp3[k] = diffval2;
					diffval2++;
				}
			}
		}

		// cout<<" vtotal.size() = "<<vtotal.size()<<endl;

		if (mp3.size()==0)
			break;
	}

	// Writing on the console of the termingal vtotal vector :-
	// cout<<" printing the vtotal vector :-  "<<endl;
	// for (i=0;i<vtotal.size();i++)
	// {
	// 	cout<<" freq of "<<i<<" th set = "<<vtotLen[i]<<endl;
	// 	for (j=0;j<vtotal[i].size();j++)
	// 		cout<<vtotal[i][j]<<" ";
	// 	cout<<endl;
	// 	cout<<endl;
	// }
	// cout<<endl;

	// Writing result in the output file
	fstream outfile;  // output file
	outfile.open(outfilename);

	if (!outfile)
	{
		// cout<<" outfile is made"<<endl;
		outfile.open(outfilename,ios::out);
	}

	// Writing result in output file
	

	if (outfile.is_open())  
	{
		string tdata;
		vector<string> vstrg; // storing string from of itermset integer value

		for (i=0;i<vtotal.size();i++)
		{

			for (j=0;j<vtotal[i].size();j++)
			{
				vstrg.push_back(to_string(vtotal[i][j]));
			}

			if (vstrg.size() != 0)
			{
				sort(vstrg.begin(),vstrg.end());

				for (j=0;j<vstrg.size();j++)
				{
					if (j==0)
						tdata = vstrg[j];
					else
						tdata = tdata + " " + vstrg[j];
				}

				vstrg.clear();

				if (tdata != "")
				{
					tdata = tdata + "\n";

					cout<<tdata;

					tdata = "";
				}
			}
		}
	}

	outfile.close();

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<seconds>(stop - start);
	// cout <<" time taken for running program = "<<duration.count() << endl; 
}

	