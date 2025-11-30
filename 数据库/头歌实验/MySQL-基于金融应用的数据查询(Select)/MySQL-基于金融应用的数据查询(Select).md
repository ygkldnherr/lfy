# MySQL-基于金融应用的数据查询(Select)

## 第1关：金融应用场景介绍,查询客户主要信息

```sql
-- 1) 查询所有客户的名称、手机号和邮箱信息。查询结果按照客户编号排序。
--    请用一条SQL语句实现该查询：
select c_name, 
    c_phone, 
    c_mail
from client
order by c_id;
/*  end  of  your code  */
```

## 第2关：邮箱为null的客户

```sql
-- 2) 查询客户表(client)中邮箱信息为null的客户的编号、名称、身份证号、手机号。
--    请用一条SQL语句实现该查询：
select c_id,
    c_name,
    c_id_card,
    c_phone
from client
where c_mail is NULL;
/*  end  of  your code  */
```

## 第3关：既买了保险又买了基金的客户

```sql
-- 3) 查询既买了保险又买了基金的客户的名称、邮箱和电话。结果依c_id排序
-- 请用一条SQL语句实现该查询：
select c_name,
    c_mail,
    c_phone
from client
where c_id in (
    select pro_c_id
    from property
    where pro_type = 2
)and c_id in (
    select pro_c_id
    from property
    where pro_type = 3
)
order by c_id;
/*  end  of  your code  */
```

## 第4关：办理了储蓄卡的客户信息

```sql
-- 4) 查询办理了储蓄卡的客户名称、手机号、银行卡号。 查询结果结果依客户编号排序。
--    请用一条SQL语句实现该查询：
select c_name,
    c_phone,
    b_number
from client, bank_card
where b_type = '储蓄卡' and client.c_id = bank_card.b_c_id
order by c_id 
/*  end  of  your code  */
```

## 第5关：每份金额在30000～50000之间的理财产品

```sql
-- 5) 查询理财产品中每份金额在30000～50000之间的理财产品的编号,每份金额，理财年限，并按照金额升序排序，金额相同的按照理财年限降序排序。
--    请用一条SQL语句实现该查询：
select p_id, 
    p_amount, 
    p_year
from finances_product
where p_amount between 30000 and 50000
order by p_amount, p_year desc;
/*  end  of  your code  */
```

## 第6关：商品收益的众数

```sql
-- 6) 查询资产表中所有资产记录里商品收益的众数和它出现的次数。
--    请用一条SQL语句实现该查询：
select pro_income,
    count(*) as presence
from property
group by pro_income
having count(*) = (
    select max(fre)
    from (
        select count(*) as fre
        from property
        group by pro_income
    ) as temp
);
/*  end  of  your code  */
```

## 第7关：未购买任何理财产品的武汉居民

```sql
-- 7) 查询身份证隶属武汉市没有买过任何理财产品的客户的名称、电话号、邮箱。
--    请用一条SQL语句实现该查询：
select c_name,
    c_phone,
    c_mail
from client
where c_id_card rlike '^4201[0-9]{13}[0-9Xx]$'
and not exists (
    select *
    from property
    where client.c_id = property.pro_c_id
    and property.pro_type = 1
);
/*  end  of  your code  */
```

## 第8关：持有两张信用卡的用户

```sql
-- 8) 查询持有两张(含）以上信用卡的用户的名称、身份证号、手机号。
--    请用一条SQL语句实现该查询：
select c_name,
    c_id_card,
    c_phone
from client join bank_card on client.c_id = bank_card.b_c_id
where bank_card.b_type = '信用卡'
group by client.c_id
having count(bank_card.b_c_id) >= 2
order by c_id;
/*  end  of  your code  */
```

## 第9关：购买了货币型基金的客户信息

```sql
-- 9) 查询购买了货币型(f_type='货币型')基金的用户的名称、电话号、邮箱。
--   请用一条SQL语句实现该查询：
select c_name,
    c_phone,
    c_mail
from (
    select distinct c_id,
        c_name,
        c_phone,
        c_mail
    from client join property on client.c_id = property.pro_c_id join fund on property.pro_pif_id = fund.f_id
    where property.pro_type = 3 and fund.f_type = '货币型'
    order by client.c_id
) t
order by c_id;
/*  end  of  your code  */
```

## 第10关：投资总收益前三名的客户

```sql
-- 10) 查询当前总的可用资产收益(被冻结的资产除外)前三名的客户的名称、身份证号及其总收益，按收益降序输出，总收益命名为total_income。不考虑并列排名情形。
--    请用一条SQL语句实现该查询：

select c_name,
    c_id_card,
    sum(pro_income) as total_income
from client join property on client.c_id = property.pro_c_id
where pro_status = '可用'
group by client.c_id
order by total_income desc
limit 3;
/*  end  of  your code  */ 
```

## 第11关：黄姓客户持卡数量

```sql
-- 11) 给出黄姓用户的编号、名称、办理的银行卡的数量(没有办卡的卡数量计为0),持卡数量命名为number_of_cards,
--     按办理银行卡数量降序输出,持卡数量相同的,依客户编号排序。
-- 请用一条SQL语句实现该查询：
select c_id,
    c_name,
    count(b_c_id) as number_of_cards
from client left join bank_card on client.c_id = bank_card.b_c_id
where c_name rlike '^黄.*$'
group by client.c_id
order by number_of_cards desc,
    c_id;
/*  end  of  your code  */
```
