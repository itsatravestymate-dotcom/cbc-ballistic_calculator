#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793



double solvepitch(double maxrangeangle, int* ticks, double testpitch, int lbarrel, double dx, double dy, double dz, double base_velocity, double gravity, double drag) {


    int iterations = 40;
    int projectile_ticks;
    double interpolated_py;
    double high = testpitch;
    double low = testpitch;

    if(testpitch < maxrangeangle){
    low = 0;
    high = maxrangeangle;
    }
    else{
    low = maxrangeangle;
    high = 89.9;
    }

    while(iterations > 0)   {

        int high_arc = (testpitch > maxrangeangle);
        testpitch = (low + high)/2;
        
        
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
        double old_px = 0;
        double old_py = 0;
        double projectile_pitch = testpitchrad;
        double t = 0;
        double dyerror = 0;

        projectile_ticks = 0;
        
        while(pdistance >= 0 && projectile_ticks < 10000 )   {
            
            old_px = px;
            old_py = py;
            px+=pxvelocity;
            py+=pyvelocity;
            pxvelocity = drag*pxvelocity;
            pyvelocity = drag*pyvelocity - gravity;
            
            pdistance = absolute_dist - px;
            projectile_ticks++;
        }

        t = (absolute_dist - old_px) / (px - old_px);
        interpolated_py = old_py + t * (py - old_py);
        dyerror = interpolated_py - absolute_vert;
        
        if(!high_arc)   {
    
            if(dyerror > 0)
                high = testpitch;
            else
                low = testpitch;
            }
        else    {
        
            if(dyerror > 0)
                low = testpitch;
            else
                high = testpitch;
        }

        iterations--;
        
    
    }
    *ticks = projectile_ticks;
    return(testpitch);
}


int main()
{
    double maxrange = 0;
    double maxrangeangle = 0;
    double gravity = 0.05;
    double drag = 0.989949664;
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
    while(testpitch >= -30 && testpitch <= 90)   {
        
        double testpitchrad = testpitch*(PI/180);
        double lbarrelhp = lbarrel*cos(testpitchrad);
        double lbarrelvp = lbarrel*sin(testpitchrad);
        double pxvelocity = base_velocity*cos(testpitchrad);
        double pyvelocity = base_velocity*sin(testpitchrad);
        double px = 0;
        double py = 0;
        double old_px = 0;
        double old_py = 0;

        while(py >= 0)   {
            
            old_px = px;
            old_py = py;
            px+=pxvelocity;
            py+=pyvelocity;
            pxvelocity = drag*pxvelocity;
            pyvelocity = drag*pyvelocity - gravity;
            
        }

        double t = (0 - old_py) / (py - old_py);
        double interpolated_px = old_px + t * (px - old_px);
        
        if(interpolated_px > maxrange)  {
            maxrange = interpolated_px;
            maxrangeangle = testpitch;
        }


        
        testpitch+=0.01;
    
    }
    printf("\nMax range of %lf, roughly at %lf deg\n", maxrange, maxrangeangle);

    if(maxrange < distance)    {
        printf("Out of range!\nYour cannon has a max range of %f", maxrange);
        printf("\nClosing program...");
        exit(0);
    }
    
    double ttt;
    double final_pitch;

    testpitch = maxrangeangle-0.1;
    final_pitch = solvepitch(maxrangeangle, &ticks, testpitch, lbarrel, dx, dy, dz, base_velocity, gravity, drag);
    ttt = ticks;
    ttt = ttt/20;
    printf("\nYaw: %lf deg Pitch: %lf deg Time to Target: %d ticks (%lf seconds)", yaw, final_pitch, ticks, ttt);

    ticks = 0;
    ttt = 0;

    testpitch = maxrangeangle+0.1;
    final_pitch = solvepitch(maxrangeangle, &ticks, testpitch, lbarrel, dx, dy, dz, base_velocity, gravity, drag);
    ttt = ticks;
    ttt = ttt/20;
    printf("\nYaw: %lf deg Pitch: %lf deg Time to Target: %d ticks (%lf seconds)", yaw, final_pitch, ticks, ttt);

    return 0;
}
