#include <fstream>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;
bool firstErr = false;
typedef long long ll;

fstream cin("input.txt");
ofstream cout("output.txt");

ll gcd(ll a, ll b) {
    return b ? gcd(b, a % b) : a;
}

class Frac {
private:
    ll numer, denum;
public:
    Frac() { numer = 0, denum = 1; }
    Frac(int x) { numer = x, denum = 1; }
    Frac(int x, int y) {
        ll g = gcd(x, y);
        numer = x / g, denum = y / g;
    }
    Frac operator * (const Frac& frac) const {
        ll x = numer * frac.numer, y = denum * frac.denum;
        ll g = gcd(x, y);
        return Frac(x / g, y / g);
    }
    Frac operator + (const Frac& frac) const {
        ll x = 0, y = 0;
        if (frac.denum == denum)
            x = numer + frac.numer, y = denum;
        else {
            x = numer * frac.denum + frac.numer * denum;
            y = denum * frac.denum;
        }
        return Frac(x, y);
    }
    Frac operator - (const Frac& frac) const {
        ll x = 0, y = 0;
        if (frac.denum == denum)
            x = numer - frac.numer, y = denum;
        else {
            x = numer * frac.denum - frac.numer * denum;
            y = denum * frac.denum;
        }
        return Frac(x, y);
    }
    Frac operator / (const Frac& frac) const {
        ll x = numer * frac.denum;
        ll y = denum * frac.numer;
        return Frac(x, y);
    }
    bool operator == (const int x) const {
        if (numer / denum == x)
            return true;
        return false;
    }
    bool operator != (const int x) const {
        if (numer / denum != x)
            return true;
        return false;
    }
    bool operator < (const Frac& frac) const {
        return numer * frac.denum < frac.numer* denum;
    }
    bool operator > (const Frac& frac) const {
        return numer * frac.denum > frac.numer * denum;
    }
    bool operator >= (const Frac& frac) const {
        return numer * frac.denum >= frac.numer * denum;
    }
    void print() {
        cout << setiosflags(ios::left);
        if (numer < 0 && denum < 0)
            numer = abs(numer), denum = abs(denum);
        else
        if (denum < 0)
            numer = -numer, denum = abs(denum);
        if (denum == 1) {
            string ans = to_string(numer);
            cout << setw(6) << ans;
        }
        else {
            string ans = to_string(numer);
            ans += '/';
            ans += to_string(denum);
            cout << setw(6) << ans;
        }
    }
};

int m;
vector<vector<Frac>>a;
vector<Frac>b;
vector<Frac>c;
vector<int>inds;
vector<Frac>ans;
double apac;

void print_of_sim_table() {
    for (auto to : c)
        to.print();
    cout << endl;
    cout << setiosflags(ios::left);
    for (int i = 0; i < a.size(); i++) {
        for (auto to : a[i])
            to.print();
        cout << endl;
    }
}

void simplex_table() {
    inds.resize(b.size());
    vector<bool>used(a.size(), false);
    for (int i = 0; i < a[0].size(); i++) {
        bool ok = true;
        int k = -1;
        for (int j = 0; j < a.size(); j++) {
            if (a[j][i] != 0 && a[j][i] != 1)
                ok = false;
            if (a[j][i] == 1 && k == -1)
                k = j;
            else
            if (a[j][i] == 1 && k != -1)
                ok = false;
        }
        if (ok) {
            used[k] = true;
            inds[k] = i;
        }
    }
    for (int j = 0; j < used.size(); j++)
        if (!used[j]) {
            inds[j] = a[0].size();
            c.push_back(Frac(0));
            for (int i = 0; i < a.size(); i++)
                if (i != j)
                    a[i].push_back(Frac(0));
                else
                    a[i].push_back(Frac(1));
        }
    c.push_back(Frac(0));
    for (int i = 0; i < b.size(); i++)
        a[i].push_back(b[i]);
    print_of_sim_table();
    cout << endl;
}

void simplex_method() {
    while (true) {
        int neg_min = 0;
        for (int i = 0; i < c.size(); i++)
            if (c[neg_min] > c[i])
                neg_min = i;
        if (c[neg_min] >= Frac(0)) {
            ans.resize(m, Frac(0));
            for (int i = 0; i < inds.size(); i++)
                if (inds[i] < m)
                    ans[inds[i]] = a[i][a[i].size() - 1]; ////
            for (int i = 0; i < ans.size(); i++) {
                cout << 'x' << i + 1 << " = ";
                ans[i].print();
                cout << endl;
            }
            cout << "Sol. = ";
            c[c.size() - 1].print();
            break;
        }
        Frac mn = Frac(1e9);
        int j = 0;
        bool neg = true;
        for (int i = 0; i < a.size(); i++) {
            Frac cur = a[i][a[0].size() - 1] / a[i][neg_min];
            if (cur > 0) neg = false;
            if (cur > 0 && cur < mn) {
                mn = cur, j = i;
            }
        }
        if (neg){ cout << "The method is not applicable!";
            break;}
        else {
            inds[j] = neg_min;
            Frac lead_elem = a[j][neg_min];
            for (int i = 0; i < a[j].size(); i++)
                a[j][i] = a[j][i] / lead_elem;
            Frac cur_elem = c[neg_min];
            for (int i = 0; i < c.size(); i++)
                c[i] = c[i] - cur_elem * a[j][i];
            for (int i = 0; i < a.size(); i++) {
                if (i != j) {
                    cur_elem = a[i][neg_min];
                    for (int l = 0; l < a[i].size(); l++)
                        a[i][l] = a[i][l] - cur_elem * a[j][l];
                }
            }
            print_of_sim_table();
            cout << endl;
        }
    }
}

int main() {
    int j = -1;
    while (true) {
        string s, x;
        if (j == -1) {
            getline(cin, s);
            s += ' ';
            for (int i = 0; i < s.size(); i++) {
                if (s[i] == ' ') {
                    if (x != "")
                        c.emplace_back(stoll(x));
                    x = "";
                }
                else
                if ((s[i] >= '0' && s[i] <= '9') || s[i] == '-')
                    x += s[i];
            }
            m = c.size();
        }
        else {
            int k = 0;
            getline(cin, s);
            s += ' ';
            a.resize(a.size() + 1);
            for (int i = 0; i < s.size(); i++) {
                if (s[i] == ' ' || s[i] == '\n') {
                    if (x != "") {
                        a[j].emplace_back(stoll(x));
                        k++;
                    }
                    x = "";
                }
                else
                    x += s[i];
            }
            if (k == 1) {
                x = "";
                for (char i : s)
                    if (i != ' ')
                        x += i;
                    else
                        break;
                apac = stold(x);
                for (auto to : a[j - 1]) {
                    if (to < Frac(0))
                        firstErr = true;
                    b.push_back(to);
                }
                a.resize(j - 1);
                break;
            }
        }
        j++;
    }
    if (firstErr) cout << "The method is not applicable!";
    else {
        simplex_table();
        simplex_method();
    }
}