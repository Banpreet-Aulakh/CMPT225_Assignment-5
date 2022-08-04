#include "hash.h"
#include "Scanner.h"
#include "Parse.h"
#include "assert.h"
#include <cstdlib>
#include <stack>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

#define tabstop 4


class var {
    public:
        string key;


        // * * * add some more attributes and methods here * * * //
        int val;
        var(string key, int val) {this->key = key; this->val = val;}
};



// * * * declare more things here * * * //

    
Set<var> memmap;
int run(StmtsNode *stmts);
string printE(ExpnNode* root);
int evalExpn(ExpnNode* root);

int run(StmtsNode *stmts) { //implemented part a + short cct eval and 2 runtime errors (div by 0) + (var not found);
    StmtsNode* curr = stmts;
    string out;

    while(curr != NULL){
        assert(curr->stmt != NULL);

        if(curr->stmt->tok == printtok){
            out = printE(curr->stmt->expn);
            cout << out << endl;
        }
        else if(curr->stmt->tok == asgntok){
            var* newVar = new var(curr->stmt->ident, evalExpn(curr->stmt->expn));
            memmap.insert(newVar);
        }
        else if(curr->stmt->tok == iftok){
            if(evalExpn(curr->stmt->expn)){
                run(curr->stmt->stmts);
            }
        }
        else if(curr->stmt->tok == whiletok){
            while(evalExpn(curr->stmt->expn)){
                run(curr->stmt->stmts);
            }
        }
        curr = curr->stmts;
    }
    return 0;
}

// Desc:  Generate bracketed infix via postorder traversal
string printE(ExpnNode *root) {
    if (root == NULL) return "";
    string out = to_string(evalExpn(root));
    return out;
}

int evalExpn(ExpnNode* root){
    if (!root){
        return 0;
    }
    if(root->tok == ident){
        if(memmap.search(root->text) == NULL){
            throw runtime_error("Variable not found");
        }
        return (memmap.search(root->text))->val;
    }
    if(!root->left_operand && !root->right_operand){
        return stoi(root->text);
    }
    int left = evalExpn(root->left_operand);
    if(left == 0 && ((root->tok == asttok) || (root->tok == slashtok))){
        return 0;
    }
    int right = evalExpn(root->right_operand);

    if(right == 0 && ((root->tok == asttok) || (root->tok == slashtok))){
        throw runtime_error("div by 0");
    }
    if(root->tok == pltok){
        return left + right;
    }
    if(root->tok == mitok){
        return left - right;
    }
    if(root->tok == asttok){
        return left * right;
    }
    if(root->tok == slashtok){
        return floor((double)left / (double)right);
    }
    if(root->tok == lttok){
        return (left < right);
    }
    if(root->tok == gttok){
        return (left > right);
    }
    if(root->tok == letok){
        return (left <= right);
    }
    if(root->tok == getok){
        return (left >= right);
    }
    if(root->tok == eqtok){
        return (left == right);
    }
    if(root->tok == netok){
        return (left != right);
    }
    if(root->tok == ortok){
        return (left || right);
    }
    if(root->tok == andtok){
        return (left && right);
    }
    if(root->tok == nottok){
        return !evalExpn(root->right_operand);
    }
    return 0;
}

int main () {
    StmtsNode *pTree;
    try {
        pTree = Parse(cin);
    }
    catch(string s) {
        cout << "Parse error: " << s << endl;
        return 0;
    }

    // cout << "Parse Successful!\n";
    run(pTree);

}

