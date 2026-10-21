// 1.Brute Force Approach
// Time:- O(n²loglogn) [n x sum_of_divisors_of(n) where sum_of_divisors_of(n) is bounded by nloglogn(Grönwall's Theorem)]
// Space:- O(n) [Due to copying all the elements]
// TLE
class Solution {
public:
    bool canSort(int i,int k,vector<int>&nums,vector<int>&result){
        // Make copy of the block size k starting with index i
        vector<int>currBlock(nums.begin()+i,nums.begin()+i+k);
        // Checking every shift
        for(int s=0;s<k;s++){
            // If the current version of currBlock is sorted 
            if(is_sorted(currBlock.begin(),currBlock.end())){
                // Then append it to the result
                result.insert(result.end(),currBlock.begin(),currBlock.end());
                // And return true that this block can be sorted
                return true;
            }
            // Else just rotate by one position anc again check
            rotate(currBlock.begin(),currBlock.begin()+1,currBlock.end());
        }
        // If none of the shifts produces a sorted block then return false
        return false;
    }
    int sortableIntegers(vector<int>& nums) {
        // Determine the length of the input vector
        int n=nums.size();
        // Initialize the answer with 0
        int ans=0;
        // For every k from 1 to n
        for(int k=1;k<=n;k++){
            // Ignore the current k if it deos not divides n
            if(n%k!=0){
                continue;
            }
            // Boolean variable to determine whether this is k sortable or not
            bool valid=true;
            // Extra vector to check whether the final array is sorted or not given that all blocks are sortable
            vector<int>result;
            // Divide nums into blocks
            for(int i=0;i<n;i+=k){
                // Check whether if this block can be sorted by performing rotations only
                // If not then break immediately
                if(!canSort(i,k,nums,result)){
                    valid=false;
                    break;
                }
            }
            // If this k all blocks can be sorted and the final array is also sorted then this k is sortable
            // So add it to the answer
            if(valid&&is_sorted(result.begin(),result.end())){
                ans+=k;
            }
        }
        // Returning the fianl answer
        return ans;
    }
};
// 2.Brute Force Approach 2.0
// Time:- O(n²loglogn) [n x sum_of_divisors_of(n) where sum_of_divisors_of(n) is bounded by nloglogn(Grönwall's Theorem)]
// Space:- O(n)
// We are just not actually rotating the block every time, instead we use are index calculations
// TLE
class Solution {
public:
    bool canSort(int i,int k,vector<int>&nums,vector<int>&result){
        // Checking every shift
        for(int s=0;s<k;s++){
            // Boolean variable to determine if this block can be sorted or not
            bool isSorted=true;
            // Checking if shift this block by s, then will it be sorted
            for(int j=1;j<k;j++){
                int prev=nums[i+(s+j-1)%k];
                int curr=nums[i+(s+j)%k];
                if(prev>curr){
                    isSorted=false;
                    break;
                }
            }
            // If this block can be sorted then
            if(isSorted){
                // Formm the result
                for(int j=0;j<k;j++){
                    result.push_back(nums[i+(s+j)%k]);
                }
                // Also return true
                return true;
            }
        }
        // If none of the shifts produces a sorted block then return false
        return false;
    }
    int sortableIntegers(vector<int>& nums) {
        // Determine the length of the input vector
        int n=nums.size();
        // Initialize the answer with 0
        int ans=0;
        // For every k from 1 to n
        for(int k=1;k<=n;k++){
            // Ignore the current k if it deos not divides n
            if(n%k!=0){
                continue;
            }
            // Boolean variable to determine whether this is k sortable or not
            bool valid=true;
            // Extra vector to check whether the final array is sorted or not given that all blocks are sortable
            vector<int>result;
            // Divide nums into blocks
            for(int i=0;i<n;i+=k){
                // Check whether if this block can be sorted by performing rotations only
                // If not then break immediately
                if(!canSort(i,k,nums,result)){
                    valid=false;
                    break;
                }
            }
            // If this k all blocks can be sorted and the final array is also sorted then this k is sortable
            // So add it to the answer
            if(valid&&is_sorted(result.begin(),result.end())){
                ans+=k;
            }
        }
        // Returning the fianl answer
        return ans;
    }
};
// 3.Better Approach
// Using KMP Algo
// Time:- O(nlogn + nloglogn) [n + sum_of_divisors_of(n) where sum_of_divisors_of(n) is bounded by nloglogn(Grönwall's Theorem)]
// Space:- O(n) 
// AC
class Solution {
public:
    // Standard fucntion to build the LPS array in KMP Algo
    vector<int> build(vector<int> &s){ 
        int l=s.size(); 
        vector<int>a(l,0); 
        int pre=0,suff=1; 
        while(suff<l){ 
            if(s[pre]==s[suff]){ 
                a[suff]=pre+1; 
                pre++; 
                suff++; 
            } 
            else{ 
                if(pre==0){ 
                    a[suff]=0; 
                    suff++; 
                    
                } 
                else{ 
                    pre=a[pre-1]; 
                    
                } 
            } 
        } 
        return a; 
    }
    // Boolean function to determing that a k is valid for a block or not
    bool kmp(vector<int> &ha,vector<int> &ne,int s){
        int k=ne.size();
        vector<int>lps=build(ne);
        int j=0;
        for(int i=0;i<2*k;i++){
            int v=ha[s+(i%k)];
            while(j>0&&v!=ne[j]){
                j=lps[j-1];
            }
            if(v==ne[j]){
                j++;
            }
            if(j==k){
                return true;
            }
        }
        return false;
    }
    int sortableIntegers(vector<int>& nums) {
        // Determine the length of the input array
        int n=nums.size();
        // Make a copy of the input array
        vector<int>sorted=nums;
        // Sort the copy array
        sort(sorted.begin(),sorted.end());
        // Initialize the answer with 0
        int ans=0;
        // Trying every possible size
        for(int k=1;k<=n;k++){
            // If k does not divide n, then ignore it
            if(n%k!=0){
                continue;
            }
            // Booleans variable t determine that this k is sortable or not
            bool ok=true;
            // s denotes the starting index of every block of size k
            for(int s=0;s<n;s+=k){
                // This is the block with starting index s assuming that k is sortable
                // We need to determine that whether the original block can be rotated to form our desired block 
                vector<int>needle(sorted.begin()+s,sorted.begin()+s+k);
                // Check whether orignal block can be made to the target one by rotations
                if(!kmp(nums,needle,s)){
                    // If not then break early
                    ok=false;
                    break;
                }
            }
            // If a valid k is found then add it to the answer
            if(ok){
                ans+=k;
            }
        }
        // Returning the final answer
        return ans;
    }
};
// 4.Optimized Approach
// Time:- O(n + nloglogn) [n + sum_of_divisors_of(n) where sum_of_divisors_of(n) is bounded by nloglogn(Grönwall's Theorem)]
// Space:- O(n) [Due to the vectors used for preprocessing]
// AC
class Solution {
public:
    bool canSort(int k,int n,vector<int>&nums,vector<int>&prefMax,vector<int>&suffMin,vector<int>&prefDrops){
        // Divide nums into n/k blocks
        for(int start=0;start<n;start+=k){
            // Determine the end index of this block
            int end=start+k-1;
            // Determine the number of drops in this block
            int dropsInThisBlock=prefDrops[end]-prefDrops[start];
            // If the last element is greater than the first element than this is also considered as a drop bcz these two elements will remian adjacent no  matter the direction of rotation
            if(nums[end]>nums[start]){
                dropsInThisBlock++;
            }
            // If a sequence of numbers have more than 1 drop then they can't be sorted by just rotating no matter the direction of rotation
            if(dropsInThisBlock>1){
                return false;
            }
            // Also we need to check that if all rotated blocks can form full sorted array 
            // For that the maximum element just before the start of this block should be lesser than or equal to the minimum element that is present in the subarray [start...n-1]
            // If this condition is not followed then return false 
            if(start&&prefMax[start-1]>suffMin[start]){
                return false;
            }
        }
        // If every condition is follwed then return true
        return true;
    }
    int sortableIntegers(vector<int>& nums) {
        // Determine the length of the input vector
        int n=nums.size();
        // Initialize the answer with 0
        int ans=0;
        // Initialize the vectors which are going to be used in preprocessing
        // prefMax[i] denotes the maximum element among the elements present at indices [0...i]
        vector<int>prefMax(n);
        // suffMin[i] denotes the minimum element among the elements present at indices [i..n-1]
        vector<int>suffMin(n);
        // prefDrops[i] denotes the descent drops that are present among the elements of indices [0...i]
        // If nums[i-1]>nums[i] for some i>0, that means that is descent
        vector<int>prefDrops(n,0);
        // Preprocessing:-
        // Calculating the prefix max array
        prefMax[0]=nums[0];
        for(int i=1;i<n;i++){
            prefMax[i]=max(prefMax[i-1],nums[i]);
        }
        // Calculating the suffix min array
        suffMin[n-1]=nums[n-1];
        for(int i=n-2;i>=0;i--){
            suffMin[i]=min(suffMin[i+1],nums[i]);
        }
        // Calculating the prefix drops array
        for(int i=1;i<n;i++){
            prefDrops[i]=prefDrops[i-1];
            if(nums[i-1]>nums[i]){
                prefDrops[i]++;
            }
        }
        // Checking every divisor of n
        for(int k=1;k*k<=n;k++){
            if(n%k!=0){
                continue;
            }
            // If this k is sortable then add it to the answer
            if(canSort(k,n,nums,prefMax,suffMin,prefDrops)){
                ans+=k;
            }
            // If n/k is sortable then add this to the answer also
            int nByk=n/k;
            if(nByk!=k){
                if(canSort(nByk,n,nums,prefMax,suffMin,prefDrops)){
                    ans+=nByk;
                }
            }
        }
        // Returning the fianl answer
        return ans;
    }
};
