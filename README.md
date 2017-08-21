# PHP Excel导出扩展
使用[libxlsxwriter](http://libxlsxwriter.github.io)库写入文件。

开发中...

## 安装
```shell
phpize
./configure --with-php-config=php-config
make
make install
```

## 使用
```PHP
$book = new ExcelBook('用户表.xlsx');
$sheet = $book->addSheet();
$sheet->addColumn('ID', 'id', ExcelColumn::FORMAT_NUMBER);
$sheet->addColumn('姓名', 'name');
$sheet->addColumn('手机', 'mobile', ExcelColumn::FORMAT_STRING);
$sheet->addColumn('金额', function($data){
    return $data['money'] - $data['frozen_money'];
}, ExcelColumn::FORMAT_NUMBER);
$data = [
    ['id' => 1, 'name' => 'ChienHo', 'mobile' => '134*******0', 'money' => 100, 'frozen_money' => 1],
    ['id' => 2, 'name' => 'Johnny', 'mobile' => '138*******3', 'money' => 13, 'frozen_money' => -1],
];
$sheet->writeByArray($data);
$book->save();
```
