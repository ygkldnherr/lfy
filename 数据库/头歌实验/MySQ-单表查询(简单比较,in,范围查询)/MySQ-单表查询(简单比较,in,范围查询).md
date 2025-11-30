# MySQ-单表查询(简单比较,in,范围查询)

## 第1关：基本查询语句

```sql
-- 任务1: 查询雇员(staff)的全部信息
-- 请在此处添加实现代码
select * from staff;

-- 任务2: 查询全部雇员(staff)的姓名(sname)、出生日期(dob)和手机号码(mobile)
-- 请在此处添加实现代码
select sname, dob, mobile from staff;

-- EOF
```

## 第2关：对查询的结果排序

```sql
-- 任务1: 写出能完成以下任务的语句：
-- 查询所有雇员的姓名,性别，手机号。按年龄降序排列，即年龄越大的越靠前。
-- 请在此处添加实现代码
select sname, gender, mobile
from staff
order by dob asc;
-- EOF
```

## 第3关：带比较条件的查询

```sql
-- 任务1: 写出能完成以下人物的sql语句：
-- 查询格力空调的型号和价格，依价格升序排列。
-- 请在此处添加实现代码
select model, price
from products
where manufacturer='格力'
order by price;
-- EOF
```

## 第4关：带 IN 关键字的查询

```sql
-- 任务1: 写出能完成以下查询任务的SQL语句:
-- 查询10，40，70号产品(空调)的型号、生产厂家和价格，依价格从低到高排序。
-- 请在此处添加实现代码
select model, manufacturer, price
from products
where pid in (10, 40, 70)
order by price;
-- EOF
```

## 第5关：带 BETWEEN AND 的范围查询

```sql
-- 任务1: 写出可以完成以下查询任务的sql语句：
-- 查询价格在2000到3000之间的所有空调的型号，生产厂家和价格。依价格从低到高排序。
-- 请在此处添加实现代码
select model, manufacturer, price
from products
where price between 2000 and 3000
order by price;
-- EOF
```
