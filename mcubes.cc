#include "mcubes.h"
int main (int argc, char* argv[]) {
    
    float threshold = strtof(argv[2], nullptr);
    ifstream inp(argv[1], ios::binary);
    if ((argc < 3))
        std::cerr << "Usage: " << argv[0] << " filename " << "threshold" << std::endl; 
    if(!inp.is_open()) {
        std::cerr << "File not opened " << argv[1] << std::endl;
        return -1;
    }
    int r, c, p; // row, column, plane
    inp >> r >> c >> p; // r=row size, c=column size, p=plane size
    inp.get(); // white-space char
    //float data[p][r][c];
    vector < vector < vector<float> > > vec_data;
    vec_data.resize(p);
    for (int i = 0; i < vec_data.size(); i++)
    {
        vec_data[i].resize(r);
        for(int j = 0; j < vec_data[i].size(); j++)
        {
            vec_data[i][j].resize(c);
        }
    }
	readFile(vec_data, inp,p, r, c);
	//cout << data[0][0][0] << endl;
	vector<coord> vec_vertex;
	vector<coord> vec_nor;
	processVector(vec_data, vec_vertex,vec_nor, threshold, p , r, c);
	//cout << vec_nor.size() << endl;
	int size = vec_vertex.size();
        cout << "PolySet \"PN\"" << endl << size << ' ' << size/3 << endl;

        // print vertex points
    	for (int l = 0; l < vec_vertex.size(); l++) {
    		cout << vec_vertex[l].p[0] << " "<<vec_vertex[l].p[1] << " "<< vec_vertex[l].p[2]<< " ";
    		cout << vec_nor[l].p[0] << " " << vec_nor[l].p[1] << " "<<vec_nor[l].p[2] << endl;
    	}
        cout << endl;
		for (int i = 0; i < vec_vertex.size(); i++) {
            cout << i << ' ';
            if ((i+1) % 3 == 0)
                cout << -1 << endl;
        }

}
void readFile(vector<vector<vector<float>>>& vec_data, ifstream& inp, int p, int r, int c){
	float val;
    // read in vals
    for(int i = 0; i < p; i++) {
        for(int j = 0; j < r; j++) { 
            for(int k = 0; k < c; k++) {
                inp.read((char*) &val, sizeof(float));
                vec_data[i][j][k] = val;
            }
        }
    }
}
void processVector(vector<vector<vector<float>>> &vec_data, vector<coord>& vec_vertex,vector<coord>& vec_nor, float threshold, int p, int r, int c){
	for(int i  = 0; i < p-1; i++){
		for(int j = 0; j < r-1; j++){
			for(int k  = 0; k < c-1; k++){
				int idx = 0 ;
				int m = 0; 
                if(vec_data[i][j][k] > threshold) {
                    idx |= (1 << 0); // push 1 to bit pos 0
                }
                if(vec_data[i][j][k+1] > threshold) {
                    idx |= (1 << 1);
                }     
                if(vec_data[i][j+1][k+1] > threshold) {
                    idx |= (1 << 2);
                }
                if(vec_data[i][j+1][k] > threshold) {
                    idx |= (1 << 3);
                }
				if(vec_data[i+1][j][k] > threshold) {
                    idx |= (1 << 4);
                }
                if(vec_data[i+1][j][k+1] > threshold) {
                    idx |= (1 << 5);
                }
				if(vec_data[i+1][j+1][k+1] > threshold) {
                    idx |= (1 << 6);
                }
                if(vec_data[i+1][j+1][k] > threshold) {
                    idx |= (1 << 7);
                }
				//cout << idx << " "; 
				insertVertex(vec_data, vec_vertex,vec_nor ,idx,i,j,k, threshold);
			}
		}
	}
}
void getNormal(vector<vector<vector<float>>>& vec_val, vector<coord>& vec_nor, int i, int j, int k, int p, int q, int r, float res){
	//cout << vec_val.size() << " "<<vec_val[i].size() <<" "<< vec_val[i][j].size() << endl;
	//if(i == 4 && j == 7 && k == 7){
		
	//}
	//cout << i << " "<<j << " "<<k << " "<<vec_val[i][j][k] << " ";
	float gi = ((i+1 < vec_val.size() ? vec_val[i+1][j][k] : vec_val[i][j][k])-(i-1 >= 0 ? vec_val[i-1][j][k] : vec_val[i][j][k]));
	//cout << gi << " ";
	gi = gi/(((i+1 < vec_val.size()) && (i-1 >= 0)) ? 2.0 : 1.0);
	//cout << gi << " "; 	
	float gj = ((j+1 < vec_val[i].size() ? vec_val[i][j+1][k] : vec_val[i][j][k])-(j-1 >= 0 ? vec_val[i][j-1][k] : vec_val[i][j][k]));
	//cout << gj << " ";
	gj = gj/(((j+1 < vec_val[i].size()) && (j-1 >= 0)) ? 2.0 : 1.0); 
	//cout << gj << " ";
	float gk = ((k+1 < vec_val[i][j].size() ? vec_val[i][j][k+1] : vec_val[i][j][k])-(k-1 >= 0 ? vec_val[i][j][k-1] : vec_val[i][j][k]));
	//cout << gk << " ";
	gk = gk/(((k+1 < vec_val[i][j].size()) && (k-1 >= 0)) ? 2.0 : 1.0); 
	//cout << gk << endl;
	//cout << p << " "<< q << " " << r << " "<<vec_val[p][q][r]<<" ";
	float gp = ((p+1 < vec_val.size() ? vec_val[p+1][q][r] : vec_val[p][q][r])-(p-1 >= 0 ? vec_val[p-1][q][r] : vec_val[p][q][r]));
	//cout << gp << " ";
	gp = gp/(((p+1 < vec_val.size()) && (p-1 >= 0)) ? 2.0 : 1.0); 
	//cout << gp << " ";
	float gq = ((q+1 < vec_val[p].size() ? vec_val[p][q+1][r] : vec_val[p][q][r])-(q-1 >= 0 ? vec_val[p][q-1][r] : vec_val[q][q][r]));
	//cout << gq << " ";
	gq = gq/(((q+1 < vec_val[p].size()) && (q-1 >= 0)) ? 2.0 : 1.0); 
	//cout<< gq << " ";
	float gr = ((r+1 < vec_val[p][q].size() ? vec_val[p][q][r+1] : vec_val[p][q][r])-(r-1 >= 0 ? vec_val[p][q][r-1] : vec_val[p][q][r]));
	//cout << gr << " ";
	gr = gr/(((r+1 < vec_val[p][q].size()) && (r-1 >= 0)) ? 2.0 : 1.0); 
	//cout << gr << endl;
	float x = gi+ res * (gp-gi);
	float y = gj+ res * (gq-gj);
	float z = gk+ res * (gr-gk);
	coord nor;
	nor.p[0] = x;
	nor.p[1] = y;
	nor.p[2] = z;
	//cout << res << endl;
	//cout << nor.p[0] << " "<<nor.p[1] << " "<<nor.p[2] << endl;
	//cout << "End" << endl;
	vec_nor.push_back(nor);
}
void insertVertex(vector<vector<vector<float>>>& vec_val, vector<coord>& vec_vertex,vector<coord>& vec_nor,int idx,int i, int j, int k, float threshold){
	int m = 0;
	while(edgeTable[idx][m] != -1) {
		//cout << edgeTable[idx][m] << " ";
        switch(edgeTable[idx][m]) {
        case 0: {
			float f1 = vec_val[i][j][k];
            float f2 = vec_val[i][j][k+1];			
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i,j,k,i,j,k+1, res);
            float k_vpt = (float)k + res; // vertex point along k
			coord c;
            c.p[0] = i;
            c.p[1] = j;
            c.p[2] = k_vpt;
			
            vec_vertex.push_back(c);
            break;
        }
        case 1: {
            float f1 = vec_val[i][j][k+1];
            float f2 = vec_val[i][j+1][k+1];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i,j,k+1,i,j+1,k+1, res);
            float j_vpt = (float)j + res;
			coord c;
            c.p[0] = i;
            c.p[1] = j_vpt;
            c.p[2] = k + 1;
            vec_vertex.push_back(c);
            break;
        }
        case 2: {
            float f2 = vec_val[i][j+1][k+1];
            float f1 = vec_val[i][j+1][k];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i,j+1,k,i,j+1,k+1, res);
            float k_vpt = (float)k + res;
			coord c;
            c.p[0] = i;
            c.p[1] = j + 1;
            c.p[2] = k_vpt;
            vec_vertex.push_back(c);
            break;
        }
        case 3: {
            float f2 = vec_val[i][j+1][k];
            float f1 = vec_val[i][j][k];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i,j,k,i,j+1,k, res);
            float j_vpt = (float)j + res;
            coord c;
			c.p[0] = i;
            c.p[1] = j_vpt;
            c.p[2] = k;
            vec_vertex.push_back(c);
            break;
        } 
        case 4: {
            float f1 = vec_val[i+1][j][k];
            float f2 = vec_val[i+1][j][k+1];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i+1,j,k,i+1,j,k+1, res);
            float k_vpt = (float)k + res;
            coord c;
			c.p[0] = i + 1;
            c.p[1] = j;
            c.p[2] = k_vpt;
            vec_vertex.push_back(c);
            break;
        }
        case 5: {
            float f1 = vec_val[i+1][j][k+1];
            float f2 = vec_val[i+1][j+1][k+1];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i+1,j,k+1,i+1,j+1,k+1, res);
            float j_vpt = (float)j + res;
			coord c;
            c.p[0] = i + 1;
            c.p[1] = j_vpt;
            c.p[2] = k + 1;
            vec_vertex.push_back(c);
            break;
        } 
        case 6: {
            float f2 = vec_val[i+1][j+1][k+1];
            float f1 = vec_val[i+1][j+1][k];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i+1,j+1,k,i+1,j+1,k+1, res);
            float k_vpt = (float)k + res;
            coord c;
			c.p[0] = i + 1;
            c.p[1] = j + 1;
            c.p[2] = k_vpt;
            vec_vertex.push_back(c);
			break;
        }
        case 7: {
            float f2 = vec_val[i+1][j+1][k];
            float f1 = vec_val[i+1][j][k];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i+1,j,k,i+1,j+1,k, res);
            float j_vpt = (float)j + res;
            coord c;
			c.p[0] = i + 1;
            c.p[1] = j_vpt;
            c.p[2] = k;
            vec_vertex.push_back(c);
            break;
        } 
        case 8: {
            float f1 = vec_val[i][j][k];
            float f2 = vec_val[i+1][j][k];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i,j,k,i+1,j,k, res);
            float i_vpt = (float)i + res;
			coord c;
            c.p[0] = i_vpt;
            c.p[1] = j;
            c.p[2] = k;
            vec_vertex.push_back(c);
            break;
        }
        case 9: {
            float f1 = vec_val[i][j][k+1];
            float f2 = vec_val[i+1][j][k+1];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i,j,k+1,i+1,j,k+1, res);
            float i_vpt = (float)i + res;
			coord c;
            c.p[0] = i_vpt;
            c.p[1] = j;
            c.p[2] = k + 1;
            vec_vertex.push_back(c);
            break;
        }
        case 10: {
            float f1 = vec_val[i][j+1][k+1];
            float f2 = vec_val[i+1][j+1][k+1];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i,j+1,k+1,i+1,j+1,k+1, res);
            float i_vpt = (float)i + res;
			coord c;
            c.p[0] = i_vpt;
            c.p[1] = j + 1;
            c.p[2] = k + 1;
            vec_vertex.push_back(c);
            break;
        }
        case 11: {
            float f1 = vec_val[i][j+1][k];
            float f2 = vec_val[i+1][j+1][k];
            float res = interpolate(f1,f2,threshold);
			getNormal(vec_val, vec_nor,i,j+1,k,i+1,j+1,k, res);
            float i_vpt = (float)i + res;
			coord c;
            c.p[0] = i_vpt;
            c.p[1] = j + 1;
            c.p[2] = k;
            vec_vertex.push_back(c);
            break;
        }
        }
        m++; // until while -1
	}
}
float interpolate(float f1, float f2, float threshold){
	float delta = (threshold - f1) / (f2 - f1); // t-val
    return delta;
}