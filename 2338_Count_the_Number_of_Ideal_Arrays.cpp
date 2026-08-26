// Problem Constraints:- n ∈ [2,1e4] and maxValue ∈ [1,1e4]
// 1st Condition:- nums[i] ∈ [1,maxValue] ∀ i ∈ [0,n)
// 2nd Condition:- (nums[i]%nums[i-1])==0 ∀ i ∈ (0,n)
// To Do:- Find the number of possible nums(% 1e9 + 7) of length n which satisifes both the above specified conditions
// Approach Explained step by step:-
// 1st Observation:- Every valid number number in an ideal array is just multiplying previous number(if exists) by some integer
// Hence the last number in an ideal array is just the product of all the integers used to form a next elemnt throughout the array 
// 2nd Observation:- We will make the array backwards that is we will fix the value at the last index,let's call the last elemnt as X
// Then we will prime factorize X and distribute power of every single prime to n positions
// Hnece, we get the hint of using Stars and bars for it
// We will calculate ways of distributing the exponent of a particular prime to n positions and as all primes are independent we will multiply ways for all primes together(% MOD)
// 3rd Observation:- If there is one or more 1's present then they should all be strictly placed in the starting like a prefix as an integer >1 b/w two 1's would make the array non-ideal
// 4th Observation:- After placing n1 1's in the starting,the problem becomes:make the ideal array of length L=(n-n1) with the modified constraint maxValue ∈ [2,1e4]
// 5th Observation:- We will iterate n1 from 0 to n-1,and keep adding the answer for the resp L=(n-n1) while taking modulo
// 6th Observation:- We will make the array backwards that is we will fix the value at the index and then consider all possibile divisors for the previous position
// 7th Observation:- Acc to 5th and 6th observation we would require a nested loop of n1 and maxValue which might give us TLE
// Consider we have placed n1 1's now let's say ways to distribute powers of prime for L be W(L),but in this we have also counted where the stars and bars have distributed 0 powers of any prime
// So we would subtract the ways where the first position after n1 1's is 1
// Which would be just W(L-1).Hence, W(L)-W(L-1)
// 8th Observation:- Now according to 7th observation,if n1 goes from 0 to n-1 then L goes form n to 1.So,
// When n1=0,L=n:W(n)-W(n-1)
// When n1=1,L=n-1:W(n-1)-W(n-2)
// When n1=2,L=n-2:W(n-2)-W(n-3)...
// When n1=n-1,L=1:W(1)-W(0)
// When we will add this,this will become a telescoping series producing the final result as W(n)-W(0).Now W(0) will just 0.So, final ans is W(n) which we can directly foin using stars and bars
// By this we removed the n1 loop
// 9th Observation:- Now we have to find power of every prime which divides our X.
// If we precompute it using standard sieve it would require sqrt time complexity
// To improve on this we would precompute spf of every number till maxValue,which drop it to logarithimic time complexity 
class Solution {
public:
    // Initializing the MOD given in the question
    const int MOD=1e9 + 7;
    // Vectors for storing factorials and inverse factorals for O(1) nCr calculation
    vector<long long>fact,invFact;
    // Vector to store spf of every number till maxValue
    vector<int>spf;
    // Binary exponentiation with MOD
    long long power(long long b,long long e){
        long long ans=1;
        b%=MOD;
        while(e>0){
            if(e%2){
                ans=(ans*b)%MOD;
            }
            b=(b*b)%MOD;
            e/=2;
        }
        return ans;
    }
    // Precomputing factorials and inverse factorials with MOD
    void preComb(int mx){
        fact.resize(mx+1);
        invFact.resize(mx+1);
        fact[0]=1;
        for(int i=1;i<=mx;i++){
            fact[i]=(fact[i-1]*i)%MOD;
        }
        // Fermat's Little Theorem
        invFact[mx]=power(fact[mx],MOD-2);
        invFact[0]=1;
        for(int i=mx-1;i>=1;i--){
            invFact[i]=(invFact[i+1]*(i+1))%MOD;
        }
    }
    // Precomputing the spf of every number
    void preSpf(int mx){
        spf.resize(mx+1);
        for(int i=1;i<=mx;i++){
            spf[i]=i;
        }
        for(int i=2;i*i<=mx;i++){
            if(spf[i]==i){
                for(int j=i*i;j<=mx;j+=i){
                    if(spf[j]==j){
                        spf[j]=i;
                    }
                }
            }
        }
    }
    // nCr function for constant time calculation
    long long nCr(int n,int r){
        if(r<0||r>n){
            return 0;
        }
        long long num=fact[n];
        long long den=(invFact[r]*invFact[n-r])%MOD;
        long long ans=(num*den)%MOD;
        return ans;
    }
    int idealArrays(int n, int maxValue) {
        // Doing the precomputation
        // Taking +15 as the highest exponent we can get is 13(of 2),hence for safety we are doing +15
        preComb(n+15);
        preSpf(maxValue);
        // Initializing the answer
        int ans=1;
        // Fixing the last value
        for(int i=2;i<=maxValue;i++){
            // Number of ideal arrays ending at i
            long long ways=1;
            int temp=i;
            // Calculation of Prime numbers and thier powers using spf 
            while(temp>1){
                int p=spf[temp];
                int e=0;
                while(temp%p==0){
                    e++;
                    temp/=p;
                }
                // Multiply the ways for every prime
                ways=(ways*nCr(n+e-1,e))%MOD;
            }
            // Add it to the final answer(% MOD)
            ans=(ans+ways)%MOD;
        }
        // Returning the answer
        return ans;
    }
};
