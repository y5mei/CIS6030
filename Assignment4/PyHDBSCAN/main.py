class UF:
    def __init__(self, N):
        self.parents = list((range(N)))
        self.size = [1] * N

    def union(self, child, parent):
        this = self.find(child)
        that = self.find(parent)

        if self.size[this] < self.size[that]:
            self.parents[this] = that
            self.size[that] += self.size[this]
        else:
            self.parents[that] = this
            self.size[this] += self.size[that]

    def find(self, x):
        while x != self.parents[x]:
            x = self.parents[x]
            self.parents[x] = self.parents[self.parents[x]]
        return x


class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right
        self.size = None


# Build the Tree
A = TreeNode(9)
C = TreeNode(8)
B = TreeNode(7)
E = TreeNode(5)
D = TreeNode(4)

C3 = TreeNode(3, D, E)
C6 = TreeNode(6, B, C)
C2 = TreeNode(2, C3, C6)
C1 = TreeNode(1, C2, A)

MCS = 2


def condense(root):
    curr = root.val
    if (not root.left and not root.right):
        root.size = 1
        return

    if root.left:
        if (not root.left.size):
            condense(root.left)
        else:
            root.size = 1 + root.left.size
            if (root.left.size < MCS):
                root.left = None

    if root.right:
        if (not root.right.size):
            condense(root.right)
        else:
            root.size += root.right.size
            if (root.right.size < MCS):
                root.right = None


condense(C1)


def myprint(node):
    print(node.val, " : ", node.size)


# Post order tranversal
def post_order(root, func):
    if not root:
        return
    if not root.left and not root.right:
        func(root)
    else:
        if root.left:
            post_order(root.left, func)
        if root.right:
            post_order(root.right, func)
        func(root)


# post_order(C1, condense)
post_order(C1, myprint)

if __name__ == "__main__":
    print("Hello World")
    uf = UF(5)
    uf.union(0,1)
    print(uf.parents)
