//
// Created by XRW's notebook on 2020/6/13.
//

#include "render.hpp"

void Render::render(int type) {
    // renderer using jittered sampling to anti-aliasing

    int w = parser->getCamera()->getWidth();
    int h = parser->getCamera()->getHeight();
    pixels.resize(w);
    for (int i = 0; i < w; i++)pixels[i].resize(h);
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
            pixels[i][j] = Vector3f();

    Camera *cam = parser->getCamera();

    // set for Depth of Field features
    // please set these features according to the scene
    cam->focus_dist = focus_dist;
    cam->len_radius = len_radius;

    // set for a naive volumetric light features
    if (volumetic)
        printf("Volumetric light on\n");
    sigma_s = 0.025; // control scattering
    sigma_a = 0.015; // control absorption
    sigma_t = sigma_s + sigma_a;

    int samples;
    if (type == 1) {
        samples = SAMPLES;
    } else if (type == 2) {
        samples = 1;
    } else {
        samples = 1;
    }

//#pragma omp parallel num_threads(8)
    {
        srand(time(nullptr));

//        for(int k=0;k<1e38;k++){
//            printf("OpenMP Test, num of thread %d\n", omp_get_thread_num());
//        }


//#pragma omp parallel for num_threads(8)

        // 1st pass of PPM
        // loop over each pixel in the image
        for (int x = 0; x < w; ++x) {
            fprintf(stderr, "\rRendering progress: %f%% ", x * 100.f / w);
            unsigned short Xi[3] = {0, 0, static_cast<unsigned short>(x * x * x)};
            for (int y = 0; y < h; ++y) {
                HitPoint hit(x, y, Vector3f(1.0, 1.0, 1.0));
                hit.weight = hit.weight / (9.f * samples);
                // jittered sampling [3*3]
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {

                        for (int s = 0; s < samples; ++s) {

                            // produce 2 random numbers in [-0.5,0.5]
                            float tmp_x = 2 * erand48(Xi), rand_x = tmp_x < 1 ? sqrt(tmp_x) - 1 : 1 - sqrt(2 - tmp_x);
                            float tmp_y = 2 * erand48(Xi), rand_y = tmp_y < 1 ? sqrt(tmp_y) - 1 : 1 - sqrt(2 - tmp_y);
                            float x_ = x + 0.5 * rand_x;
                            float y_ = y + 0.5 * rand_y;

                            // legit
//                            if (x_ < 0) x_ = 0;
//                            if (x_ >= w) x_ = w - 1;
//                            if (y_ < 0) y_ = 0;
//                            if (y_ >= img.Height()) y_ = img.Height() - 1;

                            Ray cam_ray = cam->generateRay(Vector2f(x_, y_));

                            if (type == 0 || type == 2)
                                pixels[x][y] += trace(cam_ray, hit, EPS, 0);
                            else {
                                if (!volumetic)
                                    pixels[x][y] += path_trace(cam_ray, EPS, 0, Xi);
                                else
                                    pixels[x][y] += path_trace_plus(cam_ray, EPS, 0, Xi);
                            }
                        }
                    }
                }
                pixels[x][y] = pixels[x][y] / (9.f * samples);
            }
        }
    }

    fprintf(stderr, "\rRendering progress: 100%%");
    // basic loop , only for ray tracer , debug only ,don't use!
//    for (int x = 0; x < w; ++x) {
//        for (int y = 0; y < h; ++y) {
//            Ray cam_ray = cam->generateRay(Vector2f(x, y));
//            HitPoint hit;
//            pixels[x][y] = trace(cam_ray, hit, EPS, 0);
//        }
//    }
    save_image(); // save if only for Ray Tracing / Path Tracing

    if (type != 2) return;

    // photon tracing
    tree.init(hit_points.size(), hit_points.data());

//     progressively shoot photons
    for (int i = 0; i < MAX_PPM_ITER; i++) {
        cout << "iter" << i << " total" << MAX_PPM_ITER << endl;

        float total_power = 0.f;
        for (int j = 0; j < parser->getNumLights(); j++) {
            Light *light = parser->getLight(j);
            total_power += cal_power(light->color);
        }

        // average power that a photon carries
        float photon_power = total_power / MAX_PHOTON_NUM;


        for (int i = 0; i < parser->getNumLights(); i++) {
            Light *light = parser->getLight(i);

            int num = cal_power(light->color) / photon_power;
            Vector3f photon_color = light->color / num;

            cout << num << " photons for this run " << endl;
//            omp_set_dynamic(0);
//            omp_set_num_threads(16);
//#pragma omp parallel
            {
//                srand(int(time(nullptr)) ^ omp_get_thread_num());
//#pragma omp for
                for (int j = 0; j < num; j++) {
                    if (j % 1000 == 0)
                        fprintf(stderr, "\rPhoton tracing progress: %f%% ", (j) * 100.f / num);
                    // randomly shoot photons from light
                    Vector3f ori = light->get_random_point();
//                    ori.print();
                    Vector3f dir = random_sphere();
                    Photon photon(ori, dir, photon_color);
                    trace_photon(photon, 0);
                }
                fprintf(stderr, "\rPhoton tracing progress: 100%%");
            }
        }

        update_kdtree(); // update after each iteration

        evaluate(i + 1);
    }

    save_image();
}


Vector3f Render::trace(const Ray &ray, HitPoint hit, float tmin, int bounces) {

    // basic ray tracer, considerably fast
    // It is trivial, but also stands for the 1st pass of Progressive Photon Mapping
    if (bounces > max_bounces) {
        bg_hit_points.push_back(hit);
        return parser->getBackgroundColor(ray.getDirection());
    }

    Intersection intersect_type = group->intersect(ray, hit, tmin);
    if (intersect_type == MISS) {
        bg_hit_points.push_back(hit);
        return parser->getBackgroundColor(ray.getDirection());
    }

    hit.object = group->data[hit.id];
    Vector3f N = hit.getNormal().normalized();
    Vector3f d = ray.getDirection().normalized();
    Material *material = hit.getMaterial();
    Vector3f point = ray.pointAtParameter(hit.getT());

    Vector3f f = material->getDiffuseColor();
    if (hit.material->t.valid() && hit.texture) {
        Vector2f texCoord = hit.texture_coordinate;
        Vector3f texColor = hit.material->t(texCoord[0], texCoord[1]);
        f = texColor;
    }
    if (hit.material->noise.valid()) {
        f = hit.material->noise.getColor(ray.getOrigin() + ray.getDirection() * hit.getT());
    }

    // diffusive
    HitPoint hit_diff(hit);
    hit_diff.object = hit.object;
    hit_diff.weight = hit_diff.weight * f * get_weight(material->type).x();
    hit_diff.radius2 = init_radius * init_radius;
    hit_points.push_back(hit_diff);

    // diffuse and specular , using Phong model
    Vector3f color = material->getDiffuseColor() * parser->getAmbientLight(); // Phong start with ambient light
    for (int k = 0; k < parser->getNumLights(); ++k) {
        Light *light = parser->getLight(k);
        Vector3f light_dir;
        Vector3f light_color;
        float light_dist;

        light->getIllumination(point, light_dir, light_color, light_dist);
        Ray ray_shadow(point, light_dir);
        Hit hit2;
        if (group->intersect(ray_shadow, hit2, tmin + EPS)) {
            continue;
        }
        color += (material->Shade(ray, hit, light_dir, light_color));
    }

    if (material->type == DIFF)
        return color;

    // Reflection
    Vector3f reflect_dir = reflect(N, d);
    Ray ray_reflect(point, reflect_dir);
    // reflective
    HitPoint hit_refl(hit);
    hit_refl.t = 1e38;
    hit_refl.weight = hit_refl.weight * f * get_weight(material->type).z();

    Vector3f reflect_color =
            trace(ray_reflect, hit_refl, tmin + EPS, bounces + 1) * get_spec(material->type);

    if (material->type != REFR)
        return color + reflect_color;

    // Refraction
    Vector3f N_ = N;
    Vector3f refract_dir;
    Vector3f refract_color = Vector3f::ZERO;
    float nt = material->getRefractionIndex();
    float n = 1.0;
    if (intersect_type == INSIDE) {
        N_ = -N_;
        n = nt;
        nt = 1.0;
    }
    if (refract(N_, d, n, nt, refract_dir)) {
        Ray refract_ray(point, refract_dir);
        // refractive
        HitPoint hit_refr(hit);
        hit_refr.t = 1e38;
        hit_refr.weight = hit_refr.weight * f * get_weight(material->type).w();
        refract_color =
                trace(refract_ray, hit_refr, tmin + EPS, bounces + 1) * Vector3f(0.9, 0.9, 0.9);
        float c;
        if (n <= nt)
            c = abs(Vector3f::dot(N_, d));
        else
            c = abs(Vector3f::dot(refract_dir, N_));
        float R = schlick_fresnel(c, nt, n);
        color += R * reflect_color + (1 - R) * refract_color;
        return color;
    } else {
        return color + reflect_color;
    }
}


void Render::save_image() {
    int w = parser->getCamera()->getWidth();
    int h = parser->getCamera()->getHeight();
    Image img(w, h);

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            img.SetPixel(x, y, pixels[x][y]);
        }
    }
    img.SaveBMP(output);
    printf("Image Saved.\n");
}


void Render::trace_photon(Photon &photon, int depth) {

//    cout<<"calling!\n";
    // photon tracing is the 2nd pass
    // accumulate photon power at the hit points found
    if (depth > MAX_BOUNCES)
        return;

    Ray ray(photon.origin, photon.direction);
    Hit hit;
    Intersection result = group->intersect(ray, hit, EPS);

    if (result == MISS)
        return;

    photon.P = hit.position;
    hit.object = group->data[hit.id];
    photon.object = hit.object;

    if (hit.getMaterial()->type == DIFF) tree.insertPhoton(photon);

    Photon new_photon = photon;
    new_photon.color = new_photon.color * hit.getMaterial()->getDiffuseColor();
    new_photon.origin = hit.position;

    // Russian roulette
    Vector4f weight = get_weight(hit.getMaterial()->type);
    unsigned short Xi[3] = {0, 0, static_cast<unsigned short>(hit.id * hit.id * hit.id)};
    float rand = erand48(Xi);
    float barrel1 = weight[0] + weight[1];
    float barrel2 = barrel1 + weight[2];
//    float barrel3 = barrel2 + weight[3];
    if (rand < barrel1) {
        new_photon.direction = random_cos_sphere(hit.normal);
        trace_photon(new_photon, depth + 1);
    } else if (rand < barrel2) {
        new_photon.direction = reflect(hit.normal, photon.direction);
        trace_photon(new_photon, depth + 1);
    } else {
        Vector3f N_ = hit.normal;
        float nt = hit.material->getRefractionIndex();
        float n = 1.0;
        if (result == INSIDE) {
            N_ = -N_;
            n = nt;
            nt = 1.0;
        }
        refract(N_, photon.direction, n, nt, photon.direction);
        new_photon.direction = photon.direction;
        trace_photon(new_photon, depth + 1);
    }
}


void Render::update_kdtree() {

    // update hit point map
    int num_point = 0;
    HitPoint *hit_point = tree.data(/*&*/num_point);
    for (int i = 0; i < num_point; i++)
        hit_point[i].update(ALPHA);
    tree.update();
}


void Render::evaluate(int iter) {

    // reset ray tracing result
    int w = parser->getCamera()->getWidth();
    int h = parser->getCamera()->getHeight();

    // clear photo
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            pixels[x][y] = Vector3f::ZERO;
        }
    }

    int num_point = 0;
    HitPoint *hit_point = tree.data(/*&*/num_point);
    for (int i = 0; i < num_point; i++) {
        HitPoint h = hit_point[i];
        Vector3f radiance = MAX_PPM_ITER * h.phi / (h.radius2 * iter);
        pixels[h.row][h.col] += radiance * h.weight;
    }

    for (int i = 0; i < bg_hit_points.size(); i++) {
        HitPoint h = bg_hit_points[i];
        Vector3f dir = parser->getCamera()->generateRay(Vector2f(h.col, h.row)).getDirection();
        pixels[h.row][h.col] += parser->getBackgroundColor(dir) * h.weight;
    }
}


Vector3f Render::path_trace(const Ray &ray, float tmin, int bounces, unsigned short *Xi, int E) {

    // this part uses some techniques from smallPT, a Path Tracer by Kevin Beason, 2008
    /// refering http://www.kevinbeason.com/smallpt/
    Hit hit;
    Intersection intersect_result = group->intersect(ray, hit, tmin);

    if (intersect_result == MISS || bounces > MAX_BOUNCES)
        return parser->getBackgroundColor(ray.getDirection());

    Vector3f x = hit.position;
    Vector3f n = hit.normal.normalized();
    Vector3f rd = ray.getDirection().normalized();
    Vector3f nl = n;
    if (intersect_result == INSIDE)
        nl = -n;
    Vector3f f = hit.getMaterial()->getDiffuseColor();
    if (hit.material->t.valid() && hit.texture) {
        Vector2f texCoord = hit.texture_coordinate;
        Vector3f texColor = hit.material->t(texCoord[0], texCoord[1]);
        f = texColor;
    }
    if (hit.material->noise.valid()) {
        f = hit.material->noise.getColor(ray.getOrigin() + ray.getDirection() * hit.getT());
    }

    Vector3f emission = hit.getMaterial()->getEmissionColor();

    float p = f.x() > f.y() && f.x() > f.z() ? f.x() : f.y() > f.z() ? f.y() : f.z(); // max refl
    p = 0.25 + 0.5 * p;
    if (++bounces > 5)
        if (erand48(Xi) < p)
            f = f * (1 / p);
        else
            return emission;
    // Ideal DIFFUSE reflection
    if (hit.material->type == DIFF) {
        Vector3f d = random_hemi_sphere(nl);
        return emission + f * (path_trace(Ray(x, d), tmin, bounces, Xi));
    } else if (hit.material->type == SPEC) {            // Ideal SPECULAR reflection
        return emission + f * (path_trace(Ray(x, reflect(n, rd)), tmin, bounces, Xi));
    } else if (hit.material->type == REFR) {               // Ideal dielectric REFRACTION
        Ray reflRay(x, reflect(n, rd));
        // fresnel
        float nc = 1.0;
        float nt = hit.material->getRefractionIndex();
        if (intersect_result == INSIDE) {
            nc = nt;
            nt = 1.0;
        }
        Vector3f tdir;
        bool refract_result = refract(nl, rd, nc, nt, tdir);
        if (!refract_result)
            return emission + f * (path_trace(reflRay, tmin, bounces, Xi));
        float c;
        if (intersect_result == INSIDE) {
            c = abs(Vector3f::dot(tdir, n));
        } else {
            c = abs(Vector3f::dot(nl, rd));
        }
        float Re = schlick_fresnel(c, nt, nc);
        float Tr = 1 - Re;
        float P = .25 + .5 * Re;
        float RP = Re / P, TP = Tr / (1 - P);

        // Russian roulette
        return emission + f * (bounces > 2 ? (erand48(Xi) < P ?
                                              path_trace(reflRay, tmin, bounces, Xi) * RP :
                                              path_trace(Ray(x, tdir), tmin, bounces, Xi) * TP) :
                               path_trace(reflRay, tmin, bounces, Xi) * Re +
                               path_trace(Ray(x, tdir), tmin, bounces, Xi) * Tr);
    }
}


Vector3f Render::path_trace_plus(const Ray &ray, float tmin, int bounces, unsigned short *Xi) {

    // an update version of path tracing that cope with volumetric light
    Hit hit0;
    float t_near, t_far;
    float scale = 1.0;
    float absorption = 1.0;

    // if intersect with the media
    bool intersect_media = media.intersect_0(ray, &t_near, &t_far);
    if (intersect_media) {
        Ray scatter_ray;
        float s, ms = scatter(ray, &scatter_ray, t_near, t_far, s, sigma_s);
        float prob_s = ms;
        scale = 1 / (1.0 - prob_s);
        if (prob_s >= erand48(Xi)) {

            //sample surface, or volume?
            hit0.t = t_near + s; // set nearest distance
            if (group->intersect(ray, hit0, EPS) == MISS) {
                return path_trace_plus(scatter_ray, tmin, ++bounces, Xi) * ms * (1.0 / prob_s);
            }
            scale = 1.0;
        } else {
            hit0.t = 1e38;
            if (group->intersect(ray, hit0, EPS) == MISS)
                return parser->getBackgroundColor(ray.getDirection());
        }
        if (hit0.t >= t_near) {
            float dist = (hit0.t > t_far ? t_far - t_near : hit0.t - t_near);
            absorption = exp(-sigma_t * dist);
        }
    }

    Hit hit;
    Intersection intersect_result = group->intersect(ray, hit, tmin);

    if (intersect_result == MISS || bounces > MAX_BOUNCES)
        return parser->getBackgroundColor(ray.getDirection());

    Vector3f x = hit.position;
    Vector3f n = hit.normal.normalized();
    Vector3f rd = ray.getDirection().normalized();
    Vector3f nl = n;
    if (intersect_result == INSIDE)
        nl = -n;
    Vector3f f = hit.getMaterial()->getDiffuseColor();
    if (hit.material->t.valid() && hit.texture) {
        Vector2f texCoord = hit.texture_coordinate;
        Vector3f texColor = hit.material->t(texCoord[0], texCoord[1]);
        f = texColor;
    }
    if (hit.material->noise.valid()) {
        f = hit.material->noise.getColor(ray.getOrigin() + ray.getDirection() * hit.getT());
    }

    Vector3f emission = hit.getMaterial()->getEmissionColor();

    float p = f.x() > f.y() && f.x() > f.z() ? f.x() : f.y() > f.z() ? f.y() : f.z(); // max refl
    p = 0.25 + 0.5 * p;
    if (++bounces > 5)
        if (erand48(Xi) < p)
            f = f * (1 / p);
        else
            return emission;
    if (Vector3f::dot(n, nl) > 0 || hit.getMaterial()->type != REFR) {

        // absorption light
        f = f * absorption;
        emission = emission * absorption;
    }

    // Ideal DIFFUSE reflection
    if (hit.material->type == DIFF) {
        Vector3f d = random_hemi_sphere(nl);
        return (emission + f * (path_trace_plus(Ray(x, d), tmin, bounces, Xi))) * scale;
    } else if (hit.material->type == SPEC) {            // Ideal SPECULAR reflection
        return (emission + f * (path_trace_plus(Ray(x, reflect(n, rd)), tmin, bounces, Xi))) * scale;
    } else if (hit.material->type == REFR) {               // Ideal dielectric REFRACTION
        Ray reflRay(x, reflect(n, rd));
        // fresnel
        float nc = 1.0;
        float nt = hit.material->getRefractionIndex();
        if (intersect_result == INSIDE) {
            nc = nt;
            nt = 1.0;
        }
        Vector3f tdir;
        bool refract_result = refract(nl, rd, nc, nt, tdir);
        if (!refract_result)
            return emission + f * (path_trace_plus(reflRay, tmin, bounces, Xi));
        float c;
        if (intersect_result == INSIDE) {
            c = abs(Vector3f::dot(tdir, n));
        } else {
            c = abs(Vector3f::dot(nl, rd));
        }
        float Re = schlick_fresnel(c, nt, nc);
        float Tr = 1 - Re;
        float P = .25 + .5 * Re;
        float RP = Re / P, TP = Tr / (1 - P);

        // Russian roulette
        return emission + f * (bounces > 2 ? (erand48(Xi) < P ?
                                              path_trace_plus(reflRay, tmin, bounces, Xi) * RP :
                                              path_trace_plus(Ray(x, tdir), tmin, bounces, Xi) * TP) :
                               path_trace_plus(reflRay, tmin, bounces, Xi) * Re +
                               (f * path_trace_plus(Ray(x, tdir), tmin, bounces, Xi) * Tr)) * scale;
    }
}
