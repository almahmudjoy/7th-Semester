// JetFight 3D - Ultimate Leader Jet Edition (C++98 Compatible)
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <algorithm>
#include <windows.h> // For Beep()

// Forward declarations
void display();
void update(int value);
void keyboard(unsigned char key, int x, int y);

enum PowerUpType { HEALTH, SHIELD, RAPID_FIRE, SCORE_BOOST, MULTI_SHOT };

struct PowerUp {
    float x, y;
    bool active;
    PowerUpType type;
    int timer;
};

struct Jet {
    float x, y;
    std::vector<float> bulletsY;
    std::vector<float> bulletOffsets; // For spread patterns
    bool alive;
    bool firing;
    int type; // 0=normal, 1=fast, 2=tank, 3=leader
    int health;
    int bulletPattern; // 0=single, 1=double, 2=triple
    int shotCooldown;
};

// Game state
float shipX = 0.0f;
std::vector<float> playerBulletsY;
bool playerFiring = false;
bool gameOver = false;
int score = 0;
int lives = 3;
float enemySpeed = 0.005f;
float bulletSpeed = 0.03f;
int shield = 0;
int rapidFire = 0;
int scoreMultiplier = 1;
int multiShot = 0;
int level = 1;
int healthSpawnTimer = 0;

std::vector<Jet> enemies;
std::vector<PowerUp> powerups;
std::vector<std::pair<float, float> > explosions;

void playSound(int frequency, int duration) {
    Beep(frequency, duration);
}

void drawJet(float x, float y, bool isEnemy, int type = 0) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(0.06f, 0.06f, 1.0f);

    if (isEnemy) {
        switch(type) {
            case 1: glColor3f(1.0f, 0.1f, 0.1f); break; // Fast (red)
            case 2: glColor3f(0.7f, 0.1f, 0.7f); break; // Tank (purple)
            case 3: glColor3f(1.0f, 0.8f, 0.0f); break; // Leader (gold)
            default: glColor3f(1.0f, 0.5f, 0.2f);       // Normal (orange)
        }
    } else {
        glColor3f(0.2f, 0.6f, 1.0f); // Player (blue)
    }

    // Body
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -2.0f);
    glVertex2f(-1.0f, 2.0f);
    glVertex2f(1.0f, 2.0f);
    glVertex2f(1.0f, -2.0f);
    glEnd();

    // Nose
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.0f, 2.0f);
    glVertex2f(0.0f, 4.0f);
    glVertex2f(1.0f, 2.0f);
    glEnd();

    // Wings
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(-3.0f, -1.0f);
    glVertex2f(-1.0f, -2.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(3.0f, -1.0f);
    glVertex2f(1.0f, -2.0f);
    glEnd();

    // Tail
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -2.0f);
    glVertex2f(0.0f, -3.0f);
    glVertex2f(0.5f, -2.0f);
    glEnd();

    // Leader stripes
    if (type == 3) {
        glColor3f(0.1f, 0.1f, 0.1f);
        glBegin(GL_LINES);
        glVertex2f(-2.5f, -1.5f); glVertex2f(-2.5f, 1.5f);
        glVertex2f(2.5f, -1.5f); glVertex2f(2.5f, 1.5f);
        glEnd();
    }

    // Health bars
    if (isEnemy && (type == 2 || type == 3)) {
        float maxHealth = type == 2 ? 3.0f : 5.0f;
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-3.0f, 5.0f);
        glVertex2f(-3.0f + 6.0f * enemies.back().health / maxHealth, 5.0f);
        glVertex2f(-3.0f + 6.0f * enemies.back().health / maxHealth, 6.0f);
        glVertex2f(-3.0f, 6.0f);
        glEnd();
    }

    glPopMatrix();
}

void drawBullet(float x, float y, float offsetX = 0.0f, bool isEnemy = false) {
    glPushMatrix();
    glTranslatef(x + offsetX, y, 0.0f);
    if (isEnemy) glColor3f(1.0f, 0.5f, 0.5f);
    else glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.025, 10, 10);
    glPopMatrix();
}

void drawPowerUp(float x, float y, PowerUpType type) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    switch(type) {
        case HEALTH: glColor3f(0.0f, 1.0f, 0.0f); break;
        case SHIELD: glColor3f(0.2f, 0.5f, 1.0f); break;
        case RAPID_FIRE: glColor3f(1.0f, 0.8f, 0.0f); break;
        case SCORE_BOOST: glColor3f(1.0f, 0.0f, 1.0f); break;
        case MULTI_SHOT: glColor3f(1.0f, 0.5f, 0.0f); break;
    }

    if (type != MULTI_SHOT) {
        glutSolidCube(0.05);
    } else {
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.03f, -0.03f); glVertex2f(0.03f, -0.03f); glVertex2f(0.0f, 0.03f);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.03f, 0.03f); glVertex2f(0.03f, 0.03f); glVertex2f(0.0f, -0.03f);
        glEnd();
    }

    glPopMatrix();
}

void drawExplosion(float x, float y, float size) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(1.0f, 0.7f, 0.0f);
    glutSolidSphere(size, 10, 10);
    glPopMatrix();
}

void drawShieldIndicator() {
    if (shield > 0) {
        glPushMatrix();
        glTranslatef(shipX, -0.8f, 0.0f);
        glColor4f(0.2f, 0.5f, 1.0f, 0.3f);
        glutWireSphere(0.15, 10, 10);
        glPopMatrix();
    }
}

void drawPowerUpStatus() {
    glColor4f(0.2f, 0.2f, 0.2f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(-0.98f, 0.85f); glVertex2f(-0.7f, 0.85f);
    glVertex2f(-0.7f, 0.95f); glVertex2f(-0.98f, 0.95f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.96f, 0.9f);
    std::stringstream status;
    if (rapidFire > 0) status << "Rapid: " << rapidFire/60 << "s ";
    if (multiShot > 0) status << "Multi: " << multiShot/60 << "s ";
    if (scoreMultiplier > 1) status << "x" << scoreMultiplier << " ";
    if (shield > 0) status << "Shield: " << shield;

    std::string statusStr = status.str();
    for (size_t i = 0; i < statusStr.size(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, statusStr[i]);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw player
    drawJet(shipX, -0.8f, false);
    for (size_t i = 0; i < playerBulletsY.size(); ++i) {
        drawBullet(shipX, playerBulletsY[i]);
    }
    drawShieldIndicator();

    // Draw enemies
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].alive) {
            drawJet(enemies[i].x, enemies[i].y, true, enemies[i].type);
            for (size_t j = 0; j < enemies[i].bulletsY.size(); ++j) {
                float offset = j < enemies[i].bulletOffsets.size() ? enemies[i].bulletOffsets[j] : 0.0f;
                drawBullet(enemies[i].x, enemies[i].bulletsY[j], offset, true);
            }
        }
    }

    // Draw powerups
    for (size_t i = 0; i < powerups.size(); ++i) {
        if (powerups[i].active) {
            drawPowerUp(powerups[i].x, powerups[i].y, powerups[i].type);
        }
    }

    // Draw explosions
    for (size_t i = 0; i < explosions.size(); ++i) {
        drawExplosion(explosions[i].first, explosions[i].second, 0.1f);
    }

    drawPowerUpStatus();

    // Game over screen
    if (gameOver) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.2f, 0.0f);
        std::string msg = "GAME OVER - Press R to Restart";
        for (size_t i = 0; i < msg.size(); ++i) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
        }

        glRasterPos2f(-0.15f, -0.1f);
        std::stringstream scoreMsg;
        scoreMsg << "Final Score: " << score;
        std::string scoreStr = scoreMsg.str();
        for (size_t i = 0; i < scoreStr.size(); ++i) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreStr[i]);
        }
    }

    glutSwapBuffers();
}

void spawnEnemy() {
    Jet e;
    e.x = ((rand() % 180) - 90) / 100.0f;
    e.y = 0.9f;
    e.alive = true;
    e.firing = true;
    e.shotCooldown = 0;

    // Random enemy type
    int typeRoll = rand() % 100;
    if (typeRoll < 60 - level * 5) {
        e.type = 0; e.health = 1; e.bulletPattern = 0; // Normal
    } else if (typeRoll < 85 - level * 3) {
        e.type = 1; e.health = 1; e.bulletPattern = 0; // Fast
    } else if (typeRoll < 95 - level) {
        e.type = 2; e.health = 3; e.bulletPattern = 0; // Tank
    } else {
        e.type = 3; e.health = 5; e.bulletPattern = rand() % 3; // Leader
    }

    // Initialize bullets
    switch(e.bulletPattern) {
        case 0: // Single
            e.bulletsY.push_back(e.y);
            e.bulletOffsets.push_back(0.0f);
            break;
        case 1: // Double
            e.bulletsY.push_back(e.y); e.bulletsY.push_back(e.y);
            e.bulletOffsets.push_back(-0.05f); e.bulletOffsets.push_back(0.05f);
            break;
        case 2: // Triple
            e.bulletsY.push_back(e.y); e.bulletsY.push_back(e.y); e.bulletsY.push_back(e.y);
            e.bulletOffsets.push_back(-0.1f); e.bulletOffsets.push_back(0.0f); e.bulletOffsets.push_back(0.1f);
            break;
    }

    enemies.push_back(e);
}

void spawnPowerUp(float x, float y) {
    if (rand() % 100 < 15) { // 15% chance
        PowerUp pu;
        pu.x = x;
        pu.y = y;
        pu.active = true;
        pu.timer = 300;

        int typeRoll = rand() % 100;
        if (typeRoll < 30) pu.type = HEALTH;
        else if (typeRoll < 55) pu.type = SHIELD;
        else if (typeRoll < 75) pu.type = RAPID_FIRE;
        else if (typeRoll < 90) pu.type = SCORE_BOOST;
        else pu.type = MULTI_SHOT;

        powerups.push_back(pu);
    }
}

void spawnHealthPack() {
    if (healthSpawnTimer <= 0 && rand() % 100 < 10) {
        PowerUp hp;
        hp.x = ((rand() % 180) - 90) / 100.0f;
        hp.y = 1.0f;
        hp.active = true;
        hp.timer = 300;
        hp.type = HEALTH;
        powerups.push_back(hp);
        healthSpawnTimer = 300;
    }
}

void updateLeaderJet(Jet& leader) {
    if (leader.shotCooldown > 0) {
        leader.shotCooldown--;
        return;
    }

    leader.bulletsY.clear();
    leader.bulletOffsets.clear();

    switch(leader.bulletPattern) {
        case 0:
            leader.bulletsY.push_back(leader.y);
            leader.bulletOffsets.push_back(0.0f);
            break;
        case 1:
            leader.bulletsY.push_back(leader.y); leader.bulletsY.push_back(leader.y);
            leader.bulletOffsets.push_back(-0.05f); leader.bulletOffsets.push_back(0.05f);
            break;
        case 2:
            leader.bulletsY.push_back(leader.y); leader.bulletsY.push_back(leader.y); leader.bulletsY.push_back(leader.y);
            leader.bulletOffsets.push_back(-0.1f); leader.bulletOffsets.push_back(0.0f); leader.bulletOffsets.push_back(0.1f);
            break;
    }

    leader.shotCooldown = 60 - level * 2;
    playSound(600, 30);
}

void update(int value) {
    if (gameOver) {
        glutPostRedisplay();
        glutTimerFunc(16, update, 0);
        return;
    }

    // Update timers
    if (healthSpawnTimer > 0) healthSpawnTimer--;
    if (rapidFire > 0) rapidFire--;
    if (multiShot > 0) multiShot--;
    if (rapidFire <= 0 && scoreMultiplier > 1) scoreMultiplier = 1;

    // Player bullets
    for (size_t i = 0; i < playerBulletsY.size(); ) {
        playerBulletsY[i] += bulletSpeed * (rapidFire > 0 ? 1.5f : 1.0f);
        if (playerBulletsY[i] > 1.0f) {
            playerBulletsY.erase(playerBulletsY.begin() + i);
        } else {
            i++;
        }
    }

    // Enemies
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i].alive) continue;

        // Movement
        switch(enemies[i].type) {
            case 0: enemies[i].y -= enemySpeed * (1.0f + level * 0.1f); break;
            case 1: enemies[i].y -= enemySpeed * (2.0f + level * 0.15f); break;
            case 2: enemies[i].y -= enemySpeed * (0.7f + level * 0.05f); break;
            case 3: enemies[i].y -= enemySpeed * (0.5f + level * 0.03f); break;
        }

        if (enemies[i].y < -1.0f) enemies[i].alive = false;

        // Leader behavior
        if (enemies[i].type == 3) {
            updateLeaderJet(enemies[i]);
        }

        // Enemy bullets
        for (size_t j = 0; j < enemies[i].bulletsY.size(); ) {
            enemies[i].bulletsY[j] -= bulletSpeed * (1.0f + level * 0.1f);
            float offset = j < enemies[i].bulletOffsets.size() ? enemies[i].bulletOffsets[j] : 0.0f;

            // Player hit
            if (fabs(enemies[i].bulletsY[j] + 0.8f) < 0.05f &&
                fabs(enemies[i].x + offset - shipX) < 0.05f) {
                if (shield > 0) {
                    shield--;
                    playSound(400, 50);
                } else {
                    lives--;
                    playSound(200, 200);
                    if (lives <= 0) {
                        gameOver = true;
                        playSound(150, 500);
                    }
                }
                enemies[i].bulletsY.erase(enemies[i].bulletsY.begin() + j);
                enemies[i].bulletOffsets.erase(enemies[i].bulletOffsets.begin() + j);
                continue;
            }

            if (enemies[i].bulletsY[j] < -1.0f) {
                enemies[i].bulletsY.erase(enemies[i].bulletsY.begin() + j);
                if (j < enemies[i].bulletOffsets.size()) {
                    enemies[i].bulletOffsets.erase(enemies[i].bulletOffsets.begin() + j);
                }
            } else {
                j++;
            }
        }

        // Check player bullets
        for (size_t j = 0; j < playerBulletsY.size(); ) {
            bool bulletHit = false;
            if (fabs(playerBulletsY[j] - enemies[i].y) < 0.1f &&
                fabs(shipX - enemies[i].x) < 0.1f) {
                enemies[i].health--;
                bulletHit = true;
                playSound(800, 30);

                if (enemies[i].health <= 0) {
                    enemies[i].alive = false;
                    explosions.push_back(std::make_pair(enemies[i].x, enemies[i].y));
                    score += (enemies[i].type + 1) * 10 * scoreMultiplier;
                    playSound(1000, 100);

                    if (enemies[i].type == 3 && rand() % 100 < 30) {
                        spawnPowerUp(enemies[i].x, enemies[i].y);
                    }
                }
            }

            if (bulletHit) {
                playerBulletsY.erase(playerBulletsY.begin() + j);
            } else {
                j++;
            }
        }
    }

    // Powerups
    for (size_t i = 0; i < powerups.size(); ) {
        if (!powerups[i].active) {
            i++;
            continue;
        }

        powerups[i].y -= 0.01f;
        powerups[i].timer--;

        if (powerups[i].y < -1.0f || powerups[i].timer <= 0) {
            powerups[i].active = false;
            i++;
            continue;
        }

        if (fabs(powerups[i].x - shipX) < 0.05f && fabs(powerups[i].y + 0.8f) < 0.05f) {
            switch(powerups[i].type) {
                case HEALTH:
                    lives = std::min(lives + 1, 5);
                    playSound(300, 100);
                    break;
                case SHIELD:
                    shield = 3;
                    playSound(600, 100);
                    break;
                case RAPID_FIRE:
                    rapidFire = 300;
                    playSound(800, 100);
                    break;
                case SCORE_BOOST:
                    scoreMultiplier = 2;
                    playSound(1000, 100);
                    break;
                case MULTI_SHOT:
                    multiShot = 450;
                    playSound(1200, 100);
                    break;
            }
            powerups[i].active = false;
        }
        i++;
    }

    // Cleanup
    std::vector<Jet> newEnemies;
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].alive) newEnemies.push_back(enemies[i]);
    }
    enemies = newEnemies;

    std::vector<PowerUp> newPowerups;
    for (size_t i = 0; i < powerups.size(); ++i) {
        if (powerups[i].active) newPowerups.push_back(powerups[i]);
    }
    powerups = newPowerups;

    if (!explosions.empty()) {
        explosions.erase(explosions.begin());
    }

    // Spawning
    level = 1 + score / 500;
    if (enemies.size() < (size_t)std::min(5 + level, 15) && rand() % 100 < 15) {
        spawnEnemy();
    }
    spawnHealthPack();

    // Update title
    std::stringstream title;
    title << "JetFight 3D - Score: " << score << " | Lives: " << lives << " | Level: " << level;
    if (shield > 0) title << " | Shield: " << shield;
    if (rapidFire > 0) title << " | Rapid Fire!";
    if (multiShot > 0) title << " | Multi-Shot!";
    if (scoreMultiplier > 1) title << " | x" << scoreMultiplier << " Score!";
    glutSetWindowTitle(title.str().c_str());

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (gameOver && key == 'r') {
        gameOver = false;
        lives = 3;
        score = 0;
        level = 1;
        shield = 0;
        rapidFire = 0;
        multiShot = 0;
        scoreMultiplier = 1;
        enemies.clear();
        powerups.clear();
        explosions.clear();
        playerBulletsY.clear();
        playSound(600, 100);
    }
    if (key == 'a' && shipX > -0.9f) shipX -= 0.1f;
    if (key == 'd' && shipX < 0.9f) shipX += 0.1f;
    if (key == ' ' && (playerBulletsY.empty() || rapidFire > 0)) {
        playerFiring = true;
        playSound(700, 30);

        if (multiShot > 0) {
            playerBulletsY.push_back(-0.6f);
            playerBulletsY.push_back(-0.6f);
            playerBulletsY.push_back(-0.6f);
        } else {
            playerBulletsY.push_back(-0.6f);
        }
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    enemies.clear();
    powerups.clear();
    explosions.clear();
    playerBulletsY.clear();
    spawnEnemy();
    playSound(500, 100);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("JetFight 3D - Score: 0 | Lives: 3");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
