#include<bits/stdc++.h>
#define fi first
#define se second
#define mp make_pair

using namespace std;
using ll=long long;
using pii=pair<int, int>;

const int N=2e5+13;
int n, m, q;

struct BIT{
    int bit[N];

    void update(int v){
    	assert(v > 0);
        while(v <= n){
            bit[v] ++;
            v += (v & -v);
        }
    }

    int getp(int v){
        int res=0;
        while(v > 0){
            res += bit[v];
            v -= (v & -v);
        }
        return res;
    }

    int get(int l, int r){
        return getp(r) - getp(l-1);
    }
} bit;

struct QR{
    int l, r, id;
};

int x[N], y[N];
int h[N];
int bu[N], bv[N]; // bus tour
int tin[N], tout[N], tdfs, cc;
vector<int> g[N];
int spt[20][N];
int up[20][N];
int low[N], par[N];
int resu[N], resv[N];
int curu[N], curv[N];
pii hehe;
int qlca[N];
int l[N], r[N];
vector<pair<int, int>> qrl[N];
vector<pair<int, int>> qrr[N];
vector<int> p[N];
bool down[N];

void scan(){
    cin >> n;
    for(int i=2; i<=n; i++){
        cin >> cc;
        g[cc].push_back(i);
    }
    cin >> m;
    for(int i=1; i<=m; i++){
        cin >> bu[i] >> bv[i];
    }
    cin >> q;
    for(int i=1; i<=q; i++){
        cin >> x[i] >> y[i];
    }
}

int lca(int u, int v){
    if(h[u] > h[v]) swap(u, v);
    int k=h[v]-h[u];
    for(int j=0; j<20; j++) if(k >> j & 1) v=spt[j][v];
    if(u==v) return u;
    for(int i=19; i>=0; i--){
        if(spt[i][u]!=spt[i][v]){
            u=spt[i][u];
            v=spt[i][v];
        }
    }
    return spt[0][u];
}

void dfs(int v){
    ++tdfs;
    tin[v]=tdfs;
    for(int c: g[v]){
        par[c]=spt[0][c]=v;
        h[c]=h[v]+1;
        for(int j=1; j<20; j++) spt[j][c]=spt[j-1][spt[j-1][c]];
        dfs(c);
    }
    tout[v]=tdfs;
}

void pre_dfs(int v){
    for(int c: g[v]){
    	pre_dfs(c);
        if(low[v] >= low[c]){
            low[v]=low[c];
            up[0][v]=up[0][c];
        }
    }
}

pii get(int v, int u){ // cnt jump from v to under u
    int ans=0;
    for(int i=19; i>=0; i--){
        if(h[up[i][v]] > h[u]){
            ans |= (1 << i);
            v=up[i][v];
        }
    }
    return {ans, v};
}

void prep(){
    dfs(1);
    for(int i=1; i<=n; i++){
    	low[i]=h[i], up[0][i]=i;
    }
    for(int i=1; i<=m; i++){
        int _lca=lca(bu[i], bv[i]);
        if(low[bu[i]] > h[_lca]){
            low[bu[i]] = h[_lca];
            up[0][bu[i]] = _lca;
        }
        if(low[bv[i]] > h[_lca]){
            low[bv[i]] = h[_lca];
            up[0][bv[i]] = _lca;
        }
    }
    pre_dfs(1);
    for(int j=1; j<20; j++){
        for(int i=1; i<=n; i++){
            up[j][i]=up[j-1][up[j-1][i]];
        }
    }
}

void solve(){
	for(int i=1; i<=q; i++){
        qlca[i]=lca(x[i], y[i]);
		hehe=get(x[i], qlca[i]);
        resu[i]=hehe.fi, curu[i]=hehe.se;
        hehe=get(y[i], qlca[i]);
        resv[i]=hehe.fi, curv[i]=hehe.se;
        qrl[tin[curu[i]]].push_back(mp(curv[i], i));
        qrr[tout[curu[i]]].push_back(mp(curv[i], i));
	}
	for(int i=1; i<=m; i++){
		p[tin[bu[i]]].push_back(tin[bv[i]]);
        p[tin[bv[i]]].push_back(tin[bu[i]]);
	}
	for(int i=1; i<=n; i++){
        for(pii c: qrl[i]){
            l[c.se]=bit.get(tin[c.fi], tout[c.fi]);
        }
        for(int c: p[i]){
            bit.update(c);
        }
        for(pii c: qrr[i]){
            r[c.se]=bit.get(tin[c.fi], tout[c.fi]);
        }
	}
	for(int i=1; i<=q; i++){
		if(tin[y[i]] <= tin[x[i]] && tin[x[i]] <= tout[y[i]]) swap(x[i], y[i]);
		if(y[i]==x[i]){
			cout << 0 << '\n';
		}
		else if(tin[x[i]] <= tin[y[i]] && tin[y[i]] <= tout[x[i]] && h[up[0][y[i]]] <= h[x[i]]){
			cout << 1 << '\n';
		} 
		else{
			int sus=resu[i]+resv[i]+(l[i]==r[i])+1;
			if(sus > 1000000) sus=-1;
	        cout << sus << '\n';
		}
	}
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    scan();
    prep();
    solve();
}

