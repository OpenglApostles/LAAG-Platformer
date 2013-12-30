#include "../include/object.h"
#include "../include/manager_space.h"

aabb2::aabb2(glm::vec2 a1, glm::vec2 b1) {
    a= a1;
    b= b1;
    float div = .5;
    c=div*(a+b);
}

bool aabb2::containsPoint(glm::vec2 p) {
    if(a.x<p.x)
        if(b.x>p.x)
            if(a.y<p.y)
                if(b.y>p.y)
                    return true;
    return false;
}

bool aabb2::intersectsaabb2(aabb2 o) {
    return
        a.x > o.b.x || b.x < o.a.x ||
        a.y > o.b.y || b.y < o.a.y;
}

object::object() {
    //ctor
}

object::object(glm::vec2 _p, glm::vec2 _a, polygon _body, double _mass, bool _movable, bool _colidable) {
    p=_p;
    a = _a;
    body = _body;
    mass = _mass;
    movable = _movable;
    colidable = _colidable;
    bound = aabb2(body.a*body.sc+p,body.b*body.sc+p);
}

object::~object() {
    //dtor
}

void object::move() {
    if(movable) {

    }
}

glm::vec2 contactPoint(glm::vec2 ax, object o1, object o2) {
    glm::vec2 p1 = o1.p ,p2 = o2.p;
    std::vector<glm::vec2> edges1 = o1.body.p, edges2 = o2.body.p;
    float max2 = glm::dot(ax,edges2[0]);
    float min1 = glm::dot(ax,edges1[0]);
    int m = edges2.size(), n = edges1.size();
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
    if(slope1<=0.00001 || slope2<=0.00001) {
        return edges2[jj]+p2;
    } else
        return edges1[ii]+p1;
}

bool poly_poly(object o1, object o2, glm::vec2 &ax, float &mtv) {
    mtv =2000000000;
    std::vector<glm::vec2> edges1= o1.body.p, normals1=o1.body.n, edges2=o2.body.p, normals2=o2.body.n;
    int n=edges1.size() , m=edges2.size();
    //precalculated normals
    glm::vec2 normals[n+m];
    //first poly
    for(int i=0; i<n; i++) {
        normals[i]=normals1[i];
        edges1[i]*=o1.body.sc;
    }
    //second poly
    for(int i=0; i<m; i++) {
        normals[i+n]=normals2[i];
        edges2[i]*=o2.body.sc;
    }
    glm::vec2 voffs = o1.p - o2.p;
    for(int j=0; j<n+m; j++) {
        float min1 = glm::dot(normals[j],edges1[0]);
        float max1 = min1;
        float min2 = glm::dot(normals[j],edges2[0]);
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
    return true;
}

bool poly_circle(object o1, object o2, glm::vec2 &ax, float &mtv) {
    mtv =2000000000;
    std::vector<glm::vec2> edges= o1.body.p, normals=o1.body.n;
    glm::vec2 voffs = o1.p - o2.p;
    normals.push_back(glm::normalize(voffs));
    int n=normals.size(), m = edges.size();
    float r = o2.body.p[0].x*o2.body.sc;
    for(int i=0; i<m; i++) {
        edges[i]*=o1.body.sc;
    }
    for(int j=0; j<n; j++) {
        float min1 = glm::dot(normals[j],edges[0]);
        float max1 = min1;
        float min2 = -r;
        float max2 = r;
        //first shape
        for (int i = 1; i < m; i++) {
            float p = glm::dot(normals[j],edges[i]);
            if (p < min1) {
                min1 = p;
            } else if (p > max1) {
                max1 = p;
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
    return true;
}

bool circle_circle(object o1, object o2, glm::vec2 &ax, float &mtv) {
    float dist = (o1.p.x - o2.p.x)*(o1.p.x - o2.p.x) + (o1.p.y - o2.p.y)*(o1.p.y - o2.p.y),
          r1 = o1.body.p[0].x * o1.body.sc, r2 = o2.body.p[0].x * o2.body.sc;
    if(dist > (r1+r2)*(r1+r2)) {
        return false;
    } else {
        dist = sqrt(dist);
        if(dist == 0){
            dist = manager_space::EPS;
        }
        ax = (o1.p - o2.p)/dist;
        mtv = r1+r2-dist;
        return true;
    }
}

bool object::check_col(object o, glm::vec2 &ax, float &mtv, glm::vec2 &contact) {
    bool collision;
    if(body.n.size() && o.body.n.size()) {//poly poly
        collision = poly_poly(*this, o, ax, mtv);
        if(collision)
        contact = contactPoint(ax,*this, o);
    } else if(!body.n.size() && o.body.n.size()) { //circle poly
        collision = poly_circle(o,*this, ax, mtv);
        ax*=-1;
        if(collision)
        contact = contactPoint(ax,*this, o);
    } else if(body.n.size() && !o.body.n.size()) { //poly circle
        collision = poly_circle(*this, o, ax, mtv);
        if(collision)
        contact = contactPoint(ax,*this, o);
    } else {//circle circle
        collision = circle_circle(*this, o, ax, mtv);
        if(collision)
        contact = p - ax*body.p[0].x*body.sc;
    }
    return collision;
}
