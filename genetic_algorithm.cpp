#include <bits/stdc++.h>

using namespace std;

const int NC = 5000, NS = 100 ;
const double LAMDA = 0.000001 ;
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;

    return fMin + f * (fMax - fMin);
}
class Point{
    double x, y, z;
    public:
        double getX(){return x;}
        double getY(){return y;}
        double getZ(){return z;}
        Point (double,double,double);


};
Point::Point(double _x, double _y, double _z){
    x = _x;
    y = _y;
    z = _z;
}
vector<Point> listOfA ,listOfB;

double findEuclideanDistance(Point p1 , Point p2){
    double dx = p1.getX() - p2.getX();
    double dy = p1.getY() - p2.getY();
    double dz = p1.getZ() - p2.getZ();

    return dx*dx + dy*dy + dz*dz ;
}

class candidate{
    double theta;
    public :
        double r[3][3] , t[3];
        void setTheta(){
            theta = 0.0 ;

            for(int i = 0 ; i < listOfA.size() ; i ++){
                double xx , yy , zz;
                double _x = listOfB[i].getX() , _y =  listOfB[i].getY(), _z =listOfB[i].getZ();
                xx = ( r[0][0]*_x + r[0][1]*_y + r[0][2]*_z ) + t[0] ;
                yy = ( r[1][0]*_x + r[1][1]*_y + r[1][2]*_z ) + t[1] ;
                zz = ( r[2][0]*_x + r[2][1]*_y + r[2][2]*_z ) + t[2] ;
                Point tb = Point(xx,yy,zz);
                theta += findEuclideanDistance(listOfA[i],tb);
            }

        }
        candidate(){
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    r[i][j] = fRand(-2.0,2.0);
            for(int i =0 ; i < 3 ; i ++)
                t[i] = fRand(-2.0,2.0);

            setTheta();

        }

        double getTheta(){
            return theta;
        }

};

bool compare(candidate x1 , candidate x2){
    return x1.getTheta() < x2.getTheta() ;
}

double progression(vector<candidate> l){
    double ret = 0 ;
    candidate m , M ;

    for(int i = 0 ; i < 3 ; i ++)
        for(int j = 0 ; j < 3 ; j ++){
            m.r[i]  [j] = m.t[i] = 100000.0 ;
            M.r[i][j] = M.t[i] = -100000.0;
        }



    for(auto c : l){
        for(int i = 0 ; i < 3 ; i ++)
            for(int j = 0 ; j < 3 ; j ++){
                m.r[i][j] = min(m.r[i][j] , c.r[i][j]);
                M.r[i][j] = max(M.r[i][j] , c.r[i][j]);

                m.t[i] = min(m.t[i] , c.t[i]);
                M.t[i] = max(M.t[i] , c.t[i]);

            }
    }
    cout << "matrix R :" << endl ;
    for(int i = 0 ; i < 3 ; i ++){
        for(int j = 0 ; j < 3 ; j ++){
            cout << M.r[i][j] - m.r[i][j] << " " ;
            ret = max(M.r[i][j]-m.r[i][j],ret);
        }
        cout << endl;
    }
    cout << "matrix T :" << endl ;

    for(int i = 0 ; i < 3 ; i ++){
        cout << M.t[i] - m.t[i] << endl;
        ret = max(M.t[i] - m.t[i] , ret);
    }
    cout <<"ret="<< ret << endl;
    cout <<"---------"<<endl;


    return ret;

}
class geneticAlgorithm{
    int nc, ns ;

    vector<candidate> listOfCandidate ;
    public :

        setupParameter(){
            nc = NC , ns = NS ;
        }
        setupCandidate(){
            for(int i = 0 ; i < nc ; i ++){
                candidate newCandidate = candidate();
                newCandidate.setTheta();
                listOfCandidate.emplace_back(newCandidate);
            }

        }
        candidate solve(){

            candidate newCandidate;

            sort(listOfCandidate.begin(),listOfCandidate.end(),compare);
            while(listOfCandidate.size() > ns) listOfCandidate.pop_back();

            do{
                newCandidate = candidate();


                int x1 = rand()%ns;
                int x2 = rand()%ns;

                for(int i = 0 ; i < 3; i ++)
                    for(int j = 0 ; j < 3;  j ++){
                        double f = fRand(0,1);
                        newCandidate.r[i][j] = f*listOfCandidate[x1].r[i][j] + (1.0-f)*listOfCandidate[x2].r[i][j];

                    }
                for(int i = 0 ; i < 3; i ++){
                    double f = fRand(0,1);
                    newCandidate.t[i] = f*listOfCandidate[x1].t[i] + (1.0-f)*listOfCandidate[x2].t[i];

                }

                newCandidate.setTheta();
                listOfCandidate.emplace_back(newCandidate);

                sort(listOfCandidate.begin(),listOfCandidate.end(),compare);
                while(listOfCandidate.size() > ns) listOfCandidate.pop_back();


            }while(progression(listOfCandidate) > LAMDA);

            return listOfCandidate[0];
        }

};



void loadData(){
    freopen ("A.txt","r",stdin);

    int numberOfA ;

    cin >> numberOfA ;

    for(int i = 0 ; i < numberOfA ; i ++){
        double x , y , z;
        cin >> x >> y >> z;

        listOfA.emplace_back(Point(x,y,z));
    }
    fclose (stdin);

    freopen ("B.txt","r",stdin);

    int numberOfB ;

    cin >> numberOfB;

    for(int i = 0 ; i < numberOfB ; i ++){
        double x , y , z;
        cin >> x >> y >> z;

        listOfB.emplace_back(Point(x,y,z));

    }

    fclose (stdin);


}
int main()
{
    srand (time(NULL));
    loadData();

    geneticAlgorithm problem ;
    problem.setupParameter();
    problem.setupCandidate();
    //cout << "!" <<endl;
    candidate sol = problem.solve();

    cout << "!!!!!!!!!!!"<<endl;
    cout << "matrix R :" << endl ;
    for(int i = 0 ; i < 3 ; i ++){
        for(int j = 0 ; j < 3 ; j ++)
            cout << sol.r[i][j] << " " ;
        cout << endl;
    }
    cout << "matrix T :" << endl ;

    for(int i = 0 ; i < 3 ; i ++)
        cout << sol.t[i] << endl;

    cout <<"---------"<<endl;
    cout << "theta:" << sol.getTheta() << endl;

}
