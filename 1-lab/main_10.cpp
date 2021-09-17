#include <iostream>
#include <vector>
#include <string>
#include <math.h>

// Electromagnetic interaction
// You have to create world and be able to add and delete particles from it
// When you add or delete particle you must recalculate all interactions

// Требуется описать мир с некоторыми частицами
// Координата/скорость задается через структуру vector3

double randd() {
    return (double) rand() / RAND_MAX;
}

struct vector3 {
    double x;
    double y;
    double z;

    vector3 &operator+=(const vector3 &a) {
        x += a.x, y += a.y, z += a.z;
        return *this;
    }

    vector3 &operator-=(const vector3 &a) {
        x -= a.x, y -= a.y, z -= a.z;
        return *this;
    }

    vector3 &operator+(const vector3 &a) {
        vector3 v = *this;
        return v += a;
    }

    vector3 &operator-(const vector3 &a) {
        vector3 v = *this;
        return v -= a;
    }

    vector3 &operator*=(double a) {
        x *= a, y *= a, z *= a;
        return *this;
    }

    vector3 operator*(double a) {
        vector3 v = *this;
        return v *= a;
    }

    vector3 &operator/=(double a) {
        x /= a, y /= a, z /= a;
        return *this;
    }

    vector3 operator/(double a) {
        vector3 v = *this;
        return v /= a;
    }

    double abs2() const {
        return x * x + y * y + z * z;
    };

    double abs() const {
        return sqrt(abs2());
    }
};

vector3 randv3() {
    vector3 v = {randd(), randd(), randd()};
    return v / v.abs();
}

// Для описания событий происходящих в мире напишите свой небольшой логгер, который умеет
// Запоминать создание, изменение и уничтожение частиц

class Logger {
public:
    Logger() = default;

    void log(const std::string &s) {
        l.push_back(s);
    }

    ~Logger() = default;

private:
    std::vector<std::string> l{};
};

// Существует базовый класс частиц, который внутри себя хранит информацию о частице
// Допишите этот родительский класс так, чтобы он мог считать количество объектов, созданных от него

class World;

class Particle {
protected:

    double mass = 0;
    double charge = 0;
    vector3 coordinate = {0, 0, 0};
    vector3 velocity = {0, 0, 0};
    vector3 force = {0, 0, 0};
    std::string name;
    // Логгер для записи событий
    Logger logger;

public:
    Particle() = default;

    explicit Particle(double energy, double mass_, double charge_) {
        mass = mass_;
        charge = charge_;
        coordinate = randv3();
        velocity = randv3() * sqrt(energy * 2. / mass);
        name = std::to_string(rand() % 100);
        logger.log("created: " + name);
    }

    // метод, который вам необходимо переопределить в дочерних классах
    // см. или вспомнить лекцию
    // внутри него напишите логику взаимодействия двух частиц (например, кулоновское)
    void applyForce(Particle *p) {
        vector3 r = coordinate - p->coordinate;
        double rf = r.abs();
        force += r * (p->charge * charge / (rf * rf * rf));
    }

    void makeStep(double dt) {
        velocity += force * dt / mass;
        force = {0, 0, 0};
        coordinate += velocity * dt;
    }

    std::string getName() const {
        return name;
    }

    std::string getEnergy() const {
        return std::to_string(mass * velocity.abs2() / 2);
    }

    friend std::ostream &operator<<(std::ostream &os, const World &w);

    ~Particle() {
        logger.log("destroyed: " + name);
    }
};

class Electron : public Particle {
public:
    explicit Electron(double energy) : Particle(energy, .1, -1.) {
        name = 'e' + name;
    }
};

class Positron : public Particle {
public:
    explicit Positron(double energy) : Particle(energy, .1, 1.) {
        name = 'p' + name;
    }
};

class Proton : public Particle {
public:
    explicit Proton(double energy) : Particle(energy, 1., 1.) {
        name = 'P' + name;
    }
};

class World {
    std::vector<Particle *> v;
public:
    void spawnPrticle(int type) {
        Particle *p = (type == 0) ? (Particle *) new Electron(randd()) :
                      (type == 1) ? (Particle *) new Proton(randd()) :
                      (type == 2) ? (Particle *) new Positron(randd()) : NULL;
        if (p) {
            v.push_back(p);
        }
    }

    void makeStep(double dt) {
        for (auto pa : v) {
            for (auto pb : v) {
                if (pa != pb) {
                    pa->applyForce(pb);
                }
            }
        }
        for (auto p:v) {
            p->makeStep(dt);
        }
    }

    ~World() {
        for (auto p : v) {
            delete p;
        }
    }

    // удалить частицу из мира
    //void deleteParticle(const Particle &p); - не понял, что от меня вообще требовали
    friend std::ostream &operator<<(std::ostream &os, const World &w);
};

std::ostream &operator<<(std::ostream &os, const vector3 &v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

std::ostream &operator<<(std::ostream &os, const World &w) {
    for (auto p : w.v) {
        os << p->getName() << ":\n\tenergy: " << p->getEnergy() << "\n\tposition: " << p->coordinate << "\n";
    }
    return os << "============================\n";
}

int main() {
    World worldMain;
    worldMain.spawnPrticle(0);
    worldMain.spawnPrticle(1);
    worldMain.spawnPrticle(2);
    worldMain.spawnPrticle(0);
    std::cout << worldMain;
    worldMain.makeStep(.1);
    std::cout << worldMain;
}
