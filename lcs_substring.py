class long:
    def lcs(self, s1, s2):
        # longest common substring
        # c[i][j]: the length of longest common string endswith s1[i],s2[j]
        # it is apprent that c[i][j] = c[i-1][j-1]+1 if s1[i]==s2[j], else c[i][j]=0
        # for those i==0 or j==0, c[i][j]=1 if  s1[i]==s2[j], else c[i][j]=0
        max_length = 0
        startidx = -1
        c = [[0 for i in range(len(s2))] for j in range(len(s1))]
        for i in range(len(s1)):
            for j in range(len(s2)):
                if s1[i] == s2[j]:
                    if i == 0 or j == 0:
                        c[i][j] = 1
                    else:
                        c[i][j] = c[i-1][j-1] + 1
                else:
                    c[i][j] = 0
                if c[i][j] > max_length:
                    max_length = c[i][j]
                    startidx = i - max_length + 1

        if startidx != -1:
            return s1[startidx:startidx+max_length]
        else:
            return ""

    def longestPalindrome(self, s):
        """
        :type s: str
        :rtype: str
        """
        length = 0
        rst = ''
        for i in range(len(s)):
            templeneven = 0
            k = min(i+1, len(s) - i)
            for j in range(1, k):
                if s[i-j] != s[i+j]:
                    break
                templeneven += 1
            if 2*templeneven+1 > length:
                length = 2*templeneven+1
                rst = s[i-templeneven:i+templeneven+1]

            templeneven = 0
            k = min(i+1, len(s) - i - 1)
            for j in range(k):
                if s[i-j] != s[i+j+1]:
                    break
                templeneven += 1
            if 2*templeneven > length:
                length = 2*templeneven
                rst = s[i-templeneven+1:i+templeneven+1]
        return rst


if __name__ == "__main__":
    s = "babad"
    l = long()
    rst = l.longestPalindrome(s)
    # rst = l.lcs('xiaozhen', "xizisheng")
    print(rst)
