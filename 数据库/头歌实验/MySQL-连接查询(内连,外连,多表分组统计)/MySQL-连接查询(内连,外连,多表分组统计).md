# MySQL-连接查询(内连,外连,多表分组统计)

## 第1关：内连接查询

```sql
-- 实现以下查询的SQL语句：
--查询2023年国庆小长假期间(9月29日到10月6日)的销售明细，包括：型号(model)，厂家(manufacturer)，数量(quantity)，单价(price)和折扣(discount)。查询结果依销售记录号(rid)排序。
-- 请在此处添加实现代码
select model, manufacturer, quantity, price, discount
from products join sales_record on products.pid =sales_record.pid
where sdate between '2023-9-29' and '2023-10-6'
order by sales_record.rid;
-- EOF
```

## 第2关：外连接查询

```sql
-- 实现以下查询的SQL语句：
-- 查询2023年国庆节小长假期间(9月29日到10月6日）"格力"空调的销售明细，包括：型号(model)，数量(qunantity)，单价(price)和折扣(discount)。没有销售记录的型号也要包括在查询结果中。查询结果按产品号(pid)排序,产品号相同时，再依销售记录号(rid)排序。
-- 请在此处添加实现代码
select products.model, sales_record.quantity, products.price, sales_record.discount
from products left join sales_record on products.pid = sales_record.pid and sdate between'2023-9-29' and '2023-10-6'
where manufacturer = '格力'
order by products.pid, sales_record.rid;
-- EOF
```

## 第3关：多表连接查询

```sql
-- 实现以下查询的SQL语句：
--统计各位销售人员在2023年国庆节小长假期间(9月29日到10月6日)期间的销售业绩(即销售总金额)，列出销售人员姓名，销售额(命名为total)。依销售额从高到低排序。
-- 请在此处添加实现代码
select sname, sum(quantity*price*discount) 'total'
from sales_record join staff on sales_record.sid = staff.sid join products on sales_record.pid = products.pid
where sdate between '2023-9-29' and '2023-10-6'
group by sales_record.sid
order by total desc;
-- EOF
```
