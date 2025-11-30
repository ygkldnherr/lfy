# MySQL-表结构与完整性约束的修改(ALTER)

## 第1关：修改表名

```sql
USE TestDb1;

#请在以下空白处添加恰当的语句，将表名your_table更改为my_table:
alter table your_table rename to my_table;
```

## 第2关：添加与删除字段

```sql
use MyDb;

#请在以下空白处添加适当的SQL代码，实现编程要求
#语句1：删除表orderDetail中的列orderDate
alter table orderDetail drop orderDate;
#语句2：添加列unitPrice
alter table orderDetail add unitPrice numeric(10, 2);
```

## 第3关：修改字段

```sql
use MyDb;

#请在以下空白处添加适当的SQL语句，实现编程要求
alter table addressBook
    modify QQ char(12),
    rename column weixin to wechat;
```

## 第4关：添加、删除与修改约束

```sql
use MyDb;
#请在以下空白处填写适当的诘句，实现编程要求。
#(1) 为表Staff添加主码
alter table Staff add primary key(staffNo);
#(2) Dept.mgrStaffNo是外码，对应的主码是Staff.staffNo,请添加这个外码，名字为FK_Dept_mgrStaffNo:
alter table Dept add constraint FK_Dept_mgrStaffNo foreign key(mgrStaffNo) references Staff(staffNo);
#(3) Staff.dept是外码，对应的主码是Dept.deptNo. 请添加这个外码，名字为FK_Staff_dept:
alter table Staff add constraint FK_Staff_dept foreign key(dept) references Dept(deptNo);
#(4) 为表Staff添加check约束，规则为：gender的值只能为F或M；约束名为CK_Staff_gender:
alter table Staff add constraint CK_Staff_gender check(gender in ('F','M'));
#(5) 为表Dept添加unique约束：deptName不允许重复。约束名为UN_Dept_deptName：
alter table Dept add constraint UN_Dept_deptName unique(deptName);
```
