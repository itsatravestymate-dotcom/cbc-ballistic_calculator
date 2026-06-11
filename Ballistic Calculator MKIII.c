#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793

int main()
{
    const double gravity = 0.05;
    const double drag = 0.989949664;
    printf("Input number of charges\n");
    double charge_number;
    scanf("%lf", &charge_number);
    printf("Input length of cannon\n");
    int lbarrel;
    scanf("%d", &lbarrel);
    printf("Input cannon XYZ\n");
    double cannon_x;
    double cannon_y;
    double cannon_z;
    scanf("%lf%lf%lf", &cannon_x, &cannon_y,  &cannon_z);
    printf("Input target XYZ\n");
    double target_x;
    double target_y;
    double target_z;
    scanf("%lf%lf%lf", &target_x, &target_y, &target_z);
    const double base_velocity = ((charge_number*40)/20);
    
    double range = 0;
    double angle;
    
    double dx = target_x - cannon_x;
    double dy = target_y - cannon_y;
    double dz = target_z - cannon_z;
    
    double distance = sqrt((dx*dx) + (dz*dz));
    printf("Distance: %f\n", distance);
    printf("Enter [Y] to go ahead with the calculation. Enter [N] to terminate program \n");
    char ahead;
    scanf(" %c", &ahead);
    switch(ahead)   {
        case 'Y':
            break;
        case 'N':
            printf("Closing program...");
            exit(0);
            break;
        default:
            printf("Invalid input!");
            exit(0);
            break;
    }
    
    int ticks;
    
    double yaw = atan2(-dz, dx) * 180.0 / PI;
    if(yaw < 0) {
        yaw = 360 + yaw;
    }
    
    double testpitch = -30;
    while(testpitch >= -30 && testpitch <= 60)   {
        
        double testpitchrad = testpitch*(PI/180);
        double lbarrelhp = lbarrel*cos(testpitchrad);
        double lbarrelvp = lbarrel*sin(testpitchrad);
        double pxvelocity = base_velocity*cos(testpitchrad);
        double pyvelocity = base_velocity*sin(testpitchrad);
        double px = 0;
        double py = 0;
        
        while(py >= 0)   {
            
            px+=pxvelocity;
            py+=pyvelocity;
            pxvelocity = drag*pxvelocity;
            pyvelocity = drag*pyvelocity - gravity;
        }
        
        if(px > range)  {
            range = px;
            angle = testpitch;
        }
        
        testpitch+=0.1;
    
    range = range - dy;
    
    }
    
    if(range < distance)    {
        printf("Out of range!\nYour cannon has a max range of %f", range);
        printf("\nClosing program...");
        exit(0);
    }
    
    testpitch = 15;
    double iterative_angle = 5;
    int iterations = 100;
    while(iterations > 0)   {
        
        double testpitchrad = testpitch*(PI/180);
        double lbarrelhp = lbarrel*cos(testpitchrad);
        double lbarrelvp = lbarrel*sin(testpitchrad);
        double absolute_dist = (sqrt((dx*dx) + (dz*dz))) - lbarrelhp;
        double absolute_vert = dy - lbarrelvp;
        double pxvelocity = base_velocity*cos(testpitchrad);
        double pyvelocity = base_velocity*sin(testpitchrad);
        double pdistance = absolute_dist;
        double px = 0;
        double py = 0;
        double projectile_pitch = testpitchrad;
        ticks = 0;
        
        while(pdistance >= 0)   {
            
            px+=pxvelocity;
            py+=pyvelocity;
            pxvelocity = drag*pxvelocity;
            pyvelocity = drag*pyvelocity - gravity;
            
            pdistance = absolute_dist - px;
            ticks++;
        }
        
        if(py > absolute_vert)  {
            
            testpitch-=iterative_angle;
        }
        else if(py < absolute_vert) {
            
            testpitch+=iterative_angle;
        }
        
        iterative_angle = iterative_angle*0.9;
        iterations--;
        
    
    }
    
    double final_pitch = testpitch;
    
    double ttt = ticks;
    ttt = ttt/20;
    
    printf("\nYaw: %lf deg Pitch: %lf deg Time to Target: %d ticks (%lf seconds)", yaw, final_pitch, ticks, ttt);
    
    return 0;
}