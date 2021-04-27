#include <stdio.h>
#include <stdlib.h>

int size;
int waiting[10];
int finish[10];
int execution_order[100];
int size_order;
struct process
{
    int id;
    int arrival_time;
    int burst_cycle;
    int priority;
};
void insert(struct process current[10],struct process ele,double w1,double w2)
{
    int i=0;
    double x=ele.burst_cycle*w1+ele.priority*w2;
    while((i<size)&&(current[i].burst_cycle*w1+current[i].priority*w2)>=x)
        i++;
    for(int j=size-1;j>=i;j--)
        current[j+1]=current[j];
    current[i]=ele;
    size++;
}
void pop(struct process current[10])
{
    for(int i=1;i<size;i++)
        current[i-1]=current[i];
    size--;
}
void solve(struct process current[10],int timer)
{
    int x=current[0].id-1;
    waiting[x]+=timer-finish[x];
    finish[x]=timer+1;
    if(current[0].burst_cycle==1)
        pop(current);
    else
        current[0].burst_cycle-=1;
    if((size_order==0)||(execution_order[size_order-1]!=x))
    {
        execution_order[size_order]=x;
        size_order++;
    }
}
void sort(struct process arr[10],int n,double w1,double w2)
{
    struct process temp;
    for(int i=0;i<(n-1);i++)
    {
        for(int j=0;j<(n-1-i);j++)
        {
            if(arr[j].arrival_time>arr[j+1].arrival_time)
            {
                struct process temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
        }
    }
}
int main()
{
    int n;
    double w1,w2;
    printf("Enter the number of processes : ");
    scanf("%d", &n);
    printf("\n");
    struct process arr[10];
    struct process current[10];
    for(int i=0;i<n;i++)
    {
        arr[i].id=i+1;
        printf("Enter arrival time of P%d : ",(i+1));
        scanf("%d", &arr[i].arrival_time);
        printf("\n");
        printf("Enter CPU burst cycle of P%d : ",(i+1));
        scanf("%d", &arr[i].burst_cycle);
        printf("\n");
        printf("Enter priority of P%d : ",(i+1));
        scanf("%d", &arr[i].priority);
        printf("\n");
    }
    printf("Enter value of w1 : ");
    scanf("%lf", &w1);
    printf("\n");
    printf("Enter value of w2 : ");
    scanf("%lf", &w2);
    printf("\n");
    sort(arr,n,w1,w2);
    for(int i=0;i<n;i++)
    {
        finish[arr[i].id-1]=arr[i].arrival_time;
        waiting[i]=0;
    }
    size=0;
    size_order=0;
    int timer=0;
    int ind=0;
    while(ind<n)
    {
        while((timer==arr[ind].arrival_time)&&(ind<n))
        {
            insert(current,arr[ind],w1,w2);
            ind++;
        }
        solve(current,timer);
        timer++;
    }
    for(int i=0;i<size;i++)
    {
        int x=current[i].id-1;
        waiting[x]+=timer-finish[x];
        timer+=current[i].burst_cycle;
        finish[x]=timer;
        if((size_order==0)||(execution_order[size_order-1]!=x))
        {
            execution_order[size_order]=x;
            size_order++;
        }
    }
    int total_time=0;
    for(int i=0;i<n;i++)
    {
        total_time+=waiting[i];
        printf("Waiting time of P%d : %d\n", (i+1),waiting[i]);
    }
    printf("\n");
    double avg=(double)total_time/(double)n;
    printf("Average waiting time : %lf\n", avg);
    printf("\n");
    printf("Process Execution Order : P%d",execution_order[0]+1);
    for(int i=1;i<size_order;i++)
        printf("->P%d",execution_order[i]+1);
    printf("\n\n");
    for(int i=0;i<n;i++)
        printf("P%d finished at : %d\n", (i+1),finish[i]);
    return 0;
}
