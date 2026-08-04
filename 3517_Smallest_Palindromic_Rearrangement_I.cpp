class Solution {
public:
    string smallestPalindrome(string s) {
        // Using a vector to store frequencies
        vector<int>freq(26,0);
        // Calculating the frequencies
        for(char c:s){
            freq[c-'a']++;
        }
        int n=s.length();
        // Initializing the answer
        string ans(n,' ');
        // Two pointers for designing the answer
        int l=0,r=n-1;
        for(int i=0;i<26;i++){
            // Add this character untill thier is enough freq to add it on both the sides
            while(freq[i]>=2){
                ans[l]=char('a'+i);
                ans[r]=char('a'+i);
                l++;
                r--;
                freq[i]-=2;
            }
            // If freq becomes 1 that means that this character has to be in the middle
            if(freq[i]==1){
                ans[n/2]=char('a'+i);
            }
        }
        // Returning the answer
        return ans;
    }
};
