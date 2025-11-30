# MySQL-试图

## 第1关：创建所有保险资产的详细记录视图

```sql
use finance1;
-- 创建包含所有保险资产记录的详细信息的视图v_insurance_detail，包括购买客户的名称、客户的身份证号、保险名称、保障项目、商品状态、商品数量、保险金额、保险年限、商品收益和购买时间。
-- 请用1条SQL语句完成上述任务：
create view v_insurance_detail as
select client.c_name,
    client.c_id_card,
    insurance.i_name,
    insurance.i_project,
    property.pro_status,
    property.pro_quantity,
    insurance.i_amount,
    insurance.i_year,
    property.pro_income,
    property.pro_purchase_time
from property
    join client on property.pro_c_id = client.c_id
    join insurance on property.pro_pif_id = insurance.i_id
where property.pro_type = 2;
/*   end  of your code  */
```

## 第2关：基于视图的查询

```sql
 -- 基于上一关创建的视图v_insurance_detail进行分组统计查询，列出每位客户的姓名，身份证号，保险投资总额(insurance_total_amount)和保险投资总收益(insurance_total_revenue),结果依保险投资总额降序排列。
--  请用一条SQL语句实现该查询：

select c_name,
    c_id_card,
    sum(i_amount * pro_quantity) as insurance_total_amount,
    sum(pro_income) as insurance_total_revenue
from v_insurance_detail
group by c_name,
    c_id_card
order by insurance_total_amount desc;
/*  end  of  your code  */
```
