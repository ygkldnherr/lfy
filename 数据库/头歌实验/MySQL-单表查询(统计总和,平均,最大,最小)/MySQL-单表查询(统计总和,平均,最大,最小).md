# MySQL-单表查询(统计总和,平均,最大,最小)

## 第1关：COUNT()函数及对计算列重命名

```sql
-- 实现以下查询的SQL语句：
-- 查询2023年国庆长假期间(9-29至10-6)共有多少员工共完成了多少笔销售交易，涉及多少个型号的空调。统计结果分别命名为：人数,笔数，型号数。
-- 请在此处添加实现代码
select count(distinct sid)'人数'，count(rid) '笔数', count(distinctpid) '型号数'
from sales_record
where sdate between '2023-9-29' and '2023-10-6';
-- EOF
```

## 第2关：SUM( )函数

```sql
-- 实现以下查询的SQL语句：
-- 查询7号员工2023年上半年共销售了几个型号的总共多少台空调？统计结果分别命名为：“型号数”、“总台数”。-- 请在此处添加实现代码
select count(distinct pid) '型号数', sum(quantity) '总台数'
from sales_record
where sdate between '2023-1-1' and '2023-6-30' and sid = 7;
-- EOF
```

## 第3关：AVG( )函数

```sql
-- 实现以下查询的SQL语句：
-- 查询产品表中制冷量3500W室内分体空调的平均价格,对均价取整，并命名为：“均价”。
-- 请在此处添加实现代码
select round(avg(price)) '均价'
from products
where model like 'KF%-35%'
-- EOF
```

## 第4关：MAX( )函数

```sql
-- 实现以下查询的SQL语句：
-- 查询产品表中制冷量7200W室内分体落地式冷暖空调的最高价,并命名为：“最高价”。
-- 请在此处添加实现代码
select max(price) '最高价'
from products
where model like 'KF%-72L%';
-- EOF
```

## 第5关：MIN( )函数

```sql
-- 实现以下查询的SQL语句：
-- 查询产品表中制冷量3500W室内分体壁挂式冷暖空调的最低价,并命名为：“最低价”。
-- 请在此处添加实现代码
select min(price) '最低价'
from products
where model like 'KF_-35G%';
-- EOF
```
