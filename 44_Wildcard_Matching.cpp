class Solution {
public:
    bool find(int i,int j,string& s,string& p,vector<vector<int>>& dp){
        //Base Case(1)--> If both the strings are exhausted then return true
        if(i<0 && j<0){
            return true;
        }
        //Base Case(2)--> If the pattern is exhausted but s still remains then we do not have a way to comapre or assign
        if(i>=0 && j<0){
            return false;
        }
        //Base Case(3)--> If the string s is exhausted but the pattern still remains then there is a possibilty
        // We can only get true if the remaining pattern only includes '*',because only '*' can be matched with empty seq
        if(i<0 && j>=0){
            //Returning false if there exists a character which is not '*'
            for(int k=0;k<=j;k++){
                if(p[k]!='*'){
                    return false;
                }
            }
            //Otherwise return true
            return true;
        }
        //Checking whether the state is already computed or not
        if(dp[i][j]!=-1){
            return dp[i][j];
        }
        //If both the characters match we can just continue to check further
        //If the pattern contains a '?' then also we can match it eith the character of string s
        if(s[i]==p[j] || p[j]=='?'){
            return dp[i][j] = find(i-1,j-1,s,p,dp);
        }
        //If the character is '*' then we have many options like assign it an empty seq,seq of length 1,2,.. etc
        //We are doing this by considering assigning '*' as empty or it consumes 1 character from s 
        //For ex if we consume 1 character 2 times,it denotes that we are considering '*' as seq of length 2 
        if(p[j]=='*'){
            return dp[i][j] = find(i-1,j,s,p,dp) || find(i,j-1,s,p,dp);
        }
        //If both of the cases doesn't return anything then the characters at i and j are lowercase english letters 
        //and are not same,hence returning false
        return dp[i][j] = false;
    }
    bool isMatch(string s, string p) {
        vector<vector<int>> dp(s.size(),vector<int>(p.size(),-1));
        return find(s.size()-1,p.size()-1,s,p,dp);
    }
};
