# PHP Excel导出扩展
使用[libxlswriter](http://libxlsxwriter.github.io)库写入文件。

开发中...

## 安装
```
phpize
./configure --with-php-config=php-config
make
make install
```

## 使用
```
$book = new ExcelBook('用户表.xlsx');
$book->setFormat(ExcelBook::TYPE_XLSX);
$sheet = $book->addSheet();
$sheet->addColumn('ID', 'id', ExcelColumn::FORMAT_NUMBER);
$sheet->addColumn('姓名', 'name');
$sheet->addColumn('手机', 'mobile', ExcelColumn::FORMAT_STRING);
$sheet->addColumn('金额', function($data){
    return $data['money'] - $data['frozen_money'];
}, ExcelColumn::FORMAT_NUMBER);
$book->save();
```
