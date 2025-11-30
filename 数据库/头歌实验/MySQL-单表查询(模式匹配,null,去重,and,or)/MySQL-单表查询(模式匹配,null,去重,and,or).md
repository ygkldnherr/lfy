# MySQL-单表查询(模式匹配,null,去重,and,or)

## 第1关：带 LIKE 的字符匹配查询

```sql
-- 实现以下查询的SQL语句：
-- 查询1.5匹，即功率3500W的冷暖空调，列出型号，生产厂家和价格，依价格从低到高排序。
-- 请在此处添加实现代码
select model, manufacturer, price
from products
where model like 'KF_%-35%'
order by price;
-- EOF
```

## 第2关：带RLIKE的正则表达式字符匹配查询

```sql
-- 实现以下查询的SQL语句：
-- 查询分体式室热泵制热冷暖双制，制冷量7200W或6000W的落地式空调的型号、生产厂家和价格，查询结果依价格从低到高排序输出。
-- 请在此处添加实现代码：
select model, manufacturer, price
from products
where model rlike '^KFR-(72|60)L.*'
order by price;
-- EOF
```

## 第3关：查询空值与去除重复结果

```sql
-- 实现以下查询的SQL语句：
-- 任务1：查查询产品表(products)表中，有多少厂家(manufacturer)的产品，一个厂家只列一次，即便该厂家有多个型号的产品。查询结果依厂家名称排序输出。
-- 请在此处添加实现代码
select distinct manufacturer 
from products
where model is not null
order by manufacturer;
-- 任务2：查询出生日期(出生日期)未填写(值为NULL)的员工编号，姓名。查询结果依姓名排序输出。
-- 请在此处添加实现代码
select sid, sname
from staff
where dob is null
order by sname;
-- EOF
```

## 第4关：带 AND 与 OR 的多条件查询

```sql
-- 实现以下查询的SQL语句：
-- 查询“美的”、“格力”或 “志高”价格在3000(含)以下，功率为3500W的空调信息，列出厂家，型号，价格。查询结果依价格从低到高排序。
-- 请在此处添加实现代码
select manufacturer, model, price
from products
where manufacturer in ('美的', '格力', '志高') and price <= 3000 and model like '%35%'
order by price;
-- EOF
```
