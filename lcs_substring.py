class long:
    def lcs(self, s1, s2):
        # longest common substring
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
        rs = s[::-1]
        rst = self.lcs(s, rs)
        return rst


if __name__ == "__main__":
    s = "babad"
    l = long()
    rst = l.longestPalindrome(s)
    # rst = l.lcs('xiaozhen', "xizisheng")
    print(rst)
