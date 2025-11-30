# MySQL-触发器

## 第1关：为投资表property实现业务约束规则-根据投资类别分别引用不同表的主码

```sql
use finance1;
drop trigger if exists before_property_inserted;
-- 请在适当的地方补充代码，完成任务要求：
delimiter $$
CREATE TRIGGER before_property_inserted BEFORE INSERT ON property
FOR EACH ROW 
BEGIN
    if (NEW.pro_type not in (1, 2, 3)) then
        set @msg = concat('type ', NEW.pro_type, ' is illegal!');
        signal sqlstate '45000' set message_text = @msg;
    end if;
    if (NEW.pro_type = 1) and (select count(*) from finances_product where p_id = NEW.pro_pif_id) = 0 then
        set @msg = concat('finances product #', NEW.pro_pif_id, ' not found!');
        signal sqlstate '45000' set message_text = @msg;
    end if;
    if (NEW.pro_type = 2) and (select count(*) from insurance where i_id = NEW.pro_pif_id) = 0 then
        set @msg = concat('insurance #', NEW.pro_pif_id, ' not found!');
        signal sqlstate '45000' set message_text = @msg;
    end if;
    if (NEW.pro_type = 3) and (select count(*) from fund where f_id = NEW.pro_pif_id) = 0 then
        set @msg = concat('fund #', NEW.pro_pif_id, ' not found!');
        signal sqlstate '45000' set message_text = @msg;
    end if;


END$$
 
delimiter ;
```
