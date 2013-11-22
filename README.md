一个实现在php fpm生命周期内的缓存方案.
调用方法:
在一个文件中调用:
```php
if(empty(instache_get("time"))){
    echo "setting time:<br>";
    instache_put("time",time());
}else{
    echo "time set at:".instache_get("time");
}
```

多刷新几次看看,注意观察,这个是可以跨request生命周期的.
注意,这是最简方案,没有
