#include "Surface.hpp"


bool SurfaceGroup::hit(const Ray& r, Interval ray_t, HitRecord& rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;
    for(const auto& object: objects)
    {
        if(object->hit(r, Interval(ray_t.min, closest_so_far), temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool Sphere::hit(const Ray& r, Interval ray_t, HitRecord& rec) const
{
    Vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    if(discriminant < 0)
    {
        return false;
    }
    auto sqrt_d = std::sqrt(discriminant);

    auto root = (-b - sqrt_d) / (2.0 * a);
    if(!ray_t.surrounds(root))
    {
        root = (-b + sqrt_d) / (2.0 * a);
        if(!ray_t.surrounds(root))
        {
            return false;
        }
    }
    rec.t = root;
    rec.point = r.evaluate(rec.t);
    Vec3 outward_normal = (rec.point - center) / radius;
    rec.setFaceNormal(r, unit_vector(outward_normal));
    rec.material = material;
    return true;
}

void readFace(Vec3& vertex, Vec3& normal, std::string& token, std::vector<Vec3>& vertices, std::vector<Vec3>& normals)
{
    std::istringstream stream(token);
    std::string v;
    std::string t;
    std::string n;

    std::getline(stream, v, '/');
    std::getline(stream, t, '/');
    std::getline(stream, n, '/');

    int v_index = std::stoi(v);
    int n_index = std::stoi(n);

    vertex = vertices[v_index - 1];
    normal = normals[n_index - 1];
}


Mesh::Mesh(std::string obj_file_path, shared_ptr<Material> mat) : material{mat}
{
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::ifstream file(obj_file_path);
    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty() || line[0] == '#' || line.substr(0, 7) == "mtllib") continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if(prefix == "v")
        {
            double x;
            double y;
            double z;
            iss >> x >> y >> z;
            vertices.push_back(Vec3(x, y, z));
        }
        if(prefix == "vn")
        {
            double x;
            double y;
            double z;
            iss >> x >> y >> z;
            normals.push_back(Vec3(x, y, z));
        }
        if(prefix == "f")
        {
            std::string v1;
            std::string v2;
            std::string v3;
            iss >> v1 >> v2 >> v3;

            std::shared_ptr<Triangle> triangle = make_shared<Triangle>(Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(0, 0, 0),
                                                                       Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(0, 0, 0),
                                                                       mat);

            readFace(triangle->v0, triangle->n0, v1, vertices, normals);
            readFace(triangle->v1, triangle->n1, v2, vertices, normals);
            readFace(triangle->v2, triangle->n2, v3, vertices, normals);

            triangles.push_back(*triangle);
        }
    }
}

bool Mesh::hit(const Ray& r, Interval ray_t, HitRecord& rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;
    for(const auto& triangle: triangles)
    {
        if(triangle.hit(r, Interval(ray_t.min, closest_so_far), temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    rec.material = material;
    return hit_anything;
}

bool Triangle::hit(const Ray& r, Interval ray_t, HitRecord& rec) const
{
    Vec3 n = unit_vector(cross(v1 - v0, v2 - v0));

    double n_dot_ray_direction = dot(n, r.direction());

    //check if not parallel
    if(fabs(n_dot_ray_direction) < kEpsilon)
    {
        return false;
    }

    //d - plane
    double d = -dot(n, v0);

    //t - distance of a ray to hit record
    double t = -(dot(n, r.origin()) + d) / n_dot_ray_direction;

    //check if triangle is behind
    if(t < ray_t.min || t > ray_t.max)
    {
        return false;
    }

    //Phit - intersection point
    Vec3 Phit = r.origin() + t * r.direction();

    //in/out test
    Vec3 ne;

    Vec3 v0v1 = v1 - v0;
    Vec3 v0p = Phit - v0;
    ne = cross(v0v1, v0p);
    if(dot(n, ne) < 0)
    {
        return false;
    }

    Vec3 v2v1 = v2 - v1;
    Vec3 v1p = Phit - v1;
    ne = cross(v2v1, v1p);
    if(dot(n, ne) < 0)
    {
        return false;
    }

    Vec3 v2v0 = v0 - v2;
    Vec3 v2p = Phit - v2;
    ne = cross(v2v0, v2p);
    if(dot(n, ne) < 0)
    {
        return false;
    }
    rec.t = t;
    rec.point = r.evaluate(rec.t);
    Vec3 outward_normal = n;
    rec.setFaceNormal(r, unit_vector(outward_normal));
    return true;
}
