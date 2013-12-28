#include "../include/object.h"

object::object()
{
    //ctor
}

object::object(glm::vec2 _p, glm::vec2 _a, polygon _body, double _mass, bool _movable, bool _colidable){
    p=_p;
    a = _a;
    body = _body;
    mass = _mass;
    movable = _movable;
    colidable = _colidable;
}

object::~object()
{
    //dtor
}

void object::move(){
    if(movable){

    }
}

bool object::check_col(object o, glm::vec2 &ax, float &mtv, glm::vec2 &contact){
    mtv =2000000000;
    std::vector<glm::vec2> edges1= body.p, normals1=body.n, edges2=o.body.p, normals2=o.body.n;
    int n=edges1.size() , m=edges2.size();
    //precalculated normals
    glm::vec2 normals[n+m];
    //first poly
    for(int i=0; i<n; i++) {
        normals[i]=normals1[i];
        edges1[i]*=body.sc;
    }
    //second poly
    for(int i=0; i<m; i++) {
        normals[i+n]=normals2[i];
        edges2[i]*=o.body.sc;
    }
    glm::vec2 voffs = p - o.p;
    for(int j=0; j<n+m; j++) {
        float min1 = glm::dot(normals[j],normals1[0]);
        float max1 = min1;
        float min2 = glm::dot(normals[j],normals2[0]);
        float max2 = min2;
        //first shape
        for (int i = 1; i < n; i++) {
            float p = glm::dot(normals[j],edges1[i]);
            if (p < min1) {
                min1 = p;
            } else if (p > max1) {
                max1 = p;
            }
        }
        //second shape
        for (int i = 1; i < m; i++) {
            float p = glm::dot(normals[j],edges2[i]);
            if (p < min2) {
                min2 = p;
            } else if (p > max2) {
                max2 = p;
            }
        }
        float soffset = glm::dot(normals[j], voffs);
        min1+=soffset;
        max1+=soffset;
        float mtvl1 = max2 - min1,mtvl2= max1 - min2,mtvl=std::min(mtvl1,mtvl2);
        if(mtvl<0) {
            return false;
        } else if(mtv>mtvl) {
            mtv = mtvl;
            ax = normals[j];
            if(soffset<0) {
                ax*=-1;
            }
        }
    }
    float max2 = glm::dot(ax,edges2[0]);
    float min1 = glm::dot(ax,edges1[0]);
    int ii = 0 , jj = 0;
    //contact point
    //first shape
    for (int i = 1; i < m; i++) {
        float p = glm::dot(ax,edges2[i]);
        if (p > max2) {
            jj = i;
            max2 = p;
        }
    }
    //second shape
    for (int i = 1; i < n; i++) {
        float p = glm::dot(ax,edges1[i]);
        if (p < min1) {
            ii = i;
            min1 = p;
        }
    }
    float slope1, slope2 ;
    int next1=ii+1, next2=ii-1;
    if(ii+1==n) {
        next1 = 0;
    }
    if(ii-1==-1) {
        next2 = n-1;
    }
    //first slope
    glm::vec2 edge = edges1[next1]-edges1[ii];
    edge = glm::normalize(edge);
    slope1 = glm::dot(ax,edge);
    //second slope
    edge = edges1[next2]-edges1[ii];
    edge = glm::normalize(edge);
    slope2 = glm::dot(ax,edge);
    //check
    contact = edges1[ii]+p;
    if(slope1<=0.00001 || slope2<=0.00001) {
        contact = edges2[jj]+o.p;
    }
    return true;
}
