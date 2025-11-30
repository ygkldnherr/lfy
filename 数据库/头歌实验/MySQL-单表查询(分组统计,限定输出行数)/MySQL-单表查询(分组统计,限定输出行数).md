# MySQL-单表查询(分组统计,限定输出行数)

## 第1关：使用 limit 限制查询结果的数量

```sql
-- 实现以下查询的SQL语句：
-- 查询产品表中制冷量3500W室内分体壁挂式冷暖空调的价格个最低的三个产品的厂家，型号和价格。按价格从低到高排序。。
-- 请在此处添加实现代码
select manufacturer, model, price
from products
where model rlike '^KF.?-35G.*$' 
order by price
limit 3;
-- EOF
```

## 第2关：分组统计查询(group by)

```sql
-- 实现以下查询的SQL语句：
-- 统计每个空调厂家，制冷功率为3500W的空调平均价格(取整)。结果表的标题分别为：“厂家”，“平均价格”。查询结果依生产厂家名称排序。
-- 请在此处添加实现代码
select manufacturer '厂家', round(avg(price)) '平均价格'
from products
where model rlike '^KF.?-35.*$'
group by manufacturerorder 
order by manufacturer;
-- EOF
```

## 第3关：对分组统计的结果再筛选(having)

```sql
-- 实现以下查询的SQL语句：
-- 查询2023国庆长假期间(9月29日到10月6日)所有员工销售空调的总台数，并按销售量从高到底排序。只输出总台数超过20(含)台数的结果。总台数命名为total。
-- 请在此处添加实现代码
select sid, sum(quantity) 'total'
from sales_record
where sdate between '2023-9-29' and '2023-10-6'
group by sid
having sum(quantity) >= 20
order by total desc;
-- EOF
```
