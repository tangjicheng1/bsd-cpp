import Foundation

// Definition for singly-linked list.
public class ListNode {
    public var val: Int
    public var next: ListNode?
    public init() { self.val = 0; self.next = nil; }
    public init(_ val: Int) { self.val = val; self.next = nil; }
    public init(_ val: Int, _ next: ListNode?) { self.val = val; self.next = next; }
}

// Definition for a binary tree node.
public class TreeNode {
    public var val: Int
    public var left: TreeNode?
    public var right: TreeNode?
    public init() { self.val = 0; self.left = nil; self.right = nil; }
    public init(_ val: Int) { self.val = val; self.left = nil; self.right = nil; }
    public init(_ val: Int, _ left: TreeNode?, _ right: TreeNode?) {
        self.val = val
        self.left = left
        self.right = right
    }
}

class Solution {
    func sortedListToBST(_ head: ListNode?) -> TreeNode? {
        return sortedListToBSTHelper(head, nil)
    }
    
    private func sortedListToBSTHelper(_ head: ListNode?, _ tail: ListNode?) -> TreeNode? {
        if head === tail {
            return nil
        }
        
        var slow = head
        var fast = head
        
        while fast !== tail && fast?.next !== tail {
            slow = slow?.next
            fast = fast?.next?.next
        }
        
        let root = TreeNode(slow!.val)
        root.left = sortedListToBSTHelper(head, slow)
        root.right = sortedListToBSTHelper(slow?.next, tail)
        
        return root
    }
}

// Helper function to print the tree in level order
func printTree(_ root: TreeNode?) {
    guard let root = root else {
        print("[]")
        return
    }
    
    var queue: [TreeNode?] = [root]
    var result: [String] = []
    
    while !queue.isEmpty {
        let node = queue.removeFirst()
        if let node = node {
            result.append(String(node.val))
            queue.append(node.left)
            queue.append(node.right)
        } else {
            result.append("null")
        }
    }
    
    // Remove trailing nulls
    while let last = result.last, last == "null" {
        result.removeLast()
    }
    
    print(result)
}

// Example usage:
let node5 = ListNode(9)
let node4 = ListNode(5, node5)
let node3 = ListNode(0, node4)
let node2 = ListNode(-3, node3)
let node1 = ListNode(-10, node2)

let solution = Solution()
if let bstRoot = solution.sortedListToBST(node1) {
    print("Binary Search Tree (Level Order):")
    printTree(bstRoot)
} else {
    print("Empty list, no tree to display.")
}
