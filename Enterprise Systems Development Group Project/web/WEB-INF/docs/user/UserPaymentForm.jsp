<%-- 
    Document   : UserPaymentForm
    Created on : 27-Nov-2017, 13:16:36
    Author     : Frazer, Sasha
--%>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ include file="/resources/UserDashboardNavBar.jsp" %>
<div class="main">
    <div class="content-screen">
        <div class="title">
            <h1>Make Payment</h1>
        </div>
        <form class="form" action="${pageContext.request.contextPath}/UserController" method="post" style="text-align: center;">
            <div class="input-label">
                <label>Payment Amount</label>
            </div>
            <input class="control-group" type="number" name="amount">
            <div class="input-label">
                <label>Payment Type</label>
            </div>
            <input class="control-group" type="text" name="paymentType" maxlength="10">
            <input type="hidden" name="viewId" value="/UserMakePayment">
            <input type="hidden" name="username" value="${sessionScope.username}">
            <input class="btn" type="submit" value="Submit Payment"/>
            <input class="btn" type="reset" value="Reset">
        </form>
    </div>
</div>