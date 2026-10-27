#include<iostream>
#include<iomanip>
#include<string>
using namespace std;
double toDouble(string s)
{
    double val=0.0;
    double frac=0.0;
    int i=0;
    bool neg=false;
    if(s.size()>0&&s[0]=='-')
    {
        neg=true;
        i++;
    }
    for(;i<(int)s.size()&&s[i]!='.';i++)
    {
        if(s[i]>='0'&&s[i]<='9')
        {
            val=val*10+(s[i]-'0');
        }
    }
    if(i<(int)s.size()&&s[i]=='.')
    {
        i++;
        double div=10.0;
        for(;i<(int)s.size();i++)
        {
            if(s[i]>='0'&&s[i]<='9')
            {
                frac+=(s[i]-'0')/div;
                div*=10.0;
            }
        }
    }
    val+=frac;
    if(neg) val=-val;
    return val;
}
string intToString(int num)
{
    if(num==0) return "0";
    bool neg=false;
    if(num<0)
    {
        neg=true;
        num=-num;
    }
    string s="";
    while(num>0)
    {
        char c=char('0'+(num%10));
        s=c+s;
        num/=10;
    }
    if(neg) s="-"+s;
    return s;
}
double gradePointFromLetter(string g)
{
    for(int i=0;i<(int)g.size();i++)
    {
        if(g[i]>='a'&&g[i]<='z')
        {
            g[i]=char(g[i]-'a'+'A');
        }
    }
    if(g=="A"||g=="A+")
        return 4.00;
    if(g=="A-")
        return 3.70;
    if(g=="B+")
        return 3.30;
    if(g=="B")
        return 3.00;
    if(g=="B-")
        return 2.70;
    if(g=="C+")
        return 2.30;
    if(g=="C")
        return 2.00;
    if(g=="C-")
        return 1.70;
    if(g=="D")
        return 1.00;
    if(g=="F")
        return 0.00;
    return -1.0;
}
int main()
{
    cout<<fixed<<setprecision(2);
    cout<<setw(80)<<"---- CGPA CALCULATOR ----\n\n";
    int n;
    cout<<"Enter number of courses in this semester: ";
    cin>>n;
    if(n<=0)
    {
        cout<<"No courses entered. Exiting.\n";
        return 0;
    }
    double *credits=new double[n];
    double *gradepoints=new double[n];
    string inputGrade;
    double inputNum;
    for(int i=0;i<n;i++)
    {
        cout<<"\nCourse "<<i+1<<":\n";
        cout<<"Enter credit hours: ";
        cin>>credits[i];
        while(credits[i]<=0)
        {
            cout<<"Invalid. Enter positive credit hours: ";
            cin>>credits[i];
        }
        cout<<"Enter grade (letter e.g. A, B+, C- OR numeric grade point like 3.50): ";
        cin>>inputGrade;
        bool parsedNumber=true;
        for(int j=0;j<(int)inputGrade.size();j++)
        {
            if((inputGrade[j]<'0'||inputGrade[j]>'9')&&inputGrade[j]!='.'&&inputGrade[j]!='-')
            {
                parsedNumber=false;
                break;
            }
        }
        if(parsedNumber)
        {
            inputNum=toDouble(inputGrade);
            if(inputNum<0.0) inputNum=0.0;
            if(inputNum>4.0) inputNum=4.0;
            gradepoints[i]=inputNum;
        }
        else
        {
            double gp=gradePointFromLetter(inputGrade);
            if(gp<0.0) gp=0.0;
            gradepoints[i]=gp;
        }
    }
    double totalCredits=0.0;
    double totalGradePoints=0.0;
    for(int i=0;i<n;i++)
    {
        totalCredits+=credits[i];
        totalGradePoints+=gradepoints[i]*credits[i];
    }
    double semesterGPA=0.0;
    if(totalCredits>0.0)
    {
        semesterGPA=totalGradePoints/totalCredits;
    }
    cout<<"\n"<<setw(80)<<"---- Semester Summary ----\n\n";
    cout<<left<<setw(10)<<"Course"<<setw(12)<<"Credits"<<"GradePoint\n";
    for(int i=0;i<n;i++)
    {
       cout<<left<<setw(10)<<(string("C")+intToString(i+1))<<setw(12)<<credits[i]<<gradepoints[i]<<"\n";
    }
    cout<<"\nTotal Credits this semester: "<<totalCredits<<"\n";
    cout<<"Semester GPA: "<<semesterGPA<<"\n";
    double prevCredits,prevCGPA;
    cout<<"\nIf you have previous cumulative credits and CGPA, enter them now.\n";
    cout<<"Enter previous total credits (enter 0 if none): ";
    cin>>prevCredits;
    while(prevCredits<0)
    {
        cout<<"Invalid. Enter 0 or positive: ";
        cin>>prevCredits;
    }
    cout<<"Enter previous CGPA (enter 0 if none): ";
    cin>>prevCGPA;
    while(prevCGPA<0.0||prevCGPA>4.0)
    {
        cout<<"Invalid. Enter 0..4.0: ";
        cin>>prevCGPA;
    }
    double updatedCGPA=0.0;
    if(prevCredits<=0.0)
        updatedCGPA=semesterGPA;
    else
    {
        double prevTotalGradePoints=prevCGPA*prevCredits;
        double newTotalCredits=prevCredits+totalCredits;
        double newTotalGradePoints=prevTotalGradePoints+totalGradePoints;
        if(newTotalCredits>0.0)
            updatedCGPA=newTotalGradePoints/newTotalCredits;
        else
            updatedCGPA=0.0;
    }
    cout<<"\n---- Final Result ----\n";
    cout<<"Semester GPA: "<<semesterGPA<<"\n";
    cout<<"Updated CGPA: "<<updatedCGPA<<"\n";
    delete[] credits;
    delete[] gradepoints;
    return 0;
}